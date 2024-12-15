//-----------------------------------------------------------------------------
///
/// \brief  Multi-message-package suzpport for TDT protocol
///
///         Transfer BLOBs from one device to another.
///
/// \date   20241003
/// \author Maximilian Seesslen <mes@seesslen.net>
///
//-----------------------------------------------------------------------------


//---Includes-----------------------------------------------------------------


//---Own------------------------------

#include <tdt/mmp.hpp>


//---Implementation-----------------------------------------------------------


namespace Tdt
{


CMmpTransfer::CMmpTransfer()
{
   static int counter=0;
   m_nodeId=counter;
   counter++;
}


CMmpTransfer::~CMmpTransfer() /* virtual */
{
   return;
}


void CMmpTransferData::updateCrc()
{
   if( m_header.dataLength == 0 )
   {
      m_header.crc32Data=0xFFFFFFFF;
   }
   else
   {
      m_header.crc32Data=crc32DefaultInit( );
      m_header.crc32Data=crc32Update( m_header.crc32Data
                                       , m_data, m_header.dataLength );
      m_header.crc32Data=crc32Finalize( m_header.crc32Data );
   }
};


void CMmpTransfer::writeMMP( Tdt::EObject object, int pos, uint32_t value )
{
   //if( pos % 100 == 0 )
   {
      lDebug("MMP out: pos %d; length=%d", pos, m_data.header().dataLength);
   }
   Tdt::CMessage message( m_counterNodeId,
                         Tdt::EFunctionCode::dataBlob, 
                         object, pos, value);
   //m_socketCan << message;
   #if ! defined ( STM32 )
      emit sendMessage(message);
   #elif 0
      // TBD
      sendMessage.emitSignal( message );
   #else
      cbSendTdtMessage( message );
   #endif
}


bool CMmpTransfer::handleRx( const Tdt::CMessage& msg )
{
   lDebug( "   handle Receiver [%d]", m_nodeId );
   // lDebug("pos %d dlen:%d", (int)msg.getMmpPos(), m_data.header().dataLength);
   if( m_data.pos() == -1 )
   {
      lCritical( LDS( "MMAB", "MMP abort" ) );
      m_data.reset();
      return( false );
   }
   if( m_data.pos() && ( msg.getMmpPos() == m_data.pos() - 1 ) )
   {
      lCritical( LDS("IGRE", "Ignoring retransmit" ) );
      sendAck( m_data.pos()-1 );
      return( false );
   }
   else if( msg.getMmpPos() != m_data.pos() )
   {
      lCritical( LDS("M ODNM", "MMP order does not match:" ) );
      lDebug( "   msg %d vs. cur %d"
                , (int)msg.getMmpPos(), (int)m_data.pos() );

      m_data.reset();
      sendAck( -1 );
      return( false );
   }
   
   lDebug("Pushing data");
   m_data.data32()=msg.getTdtValue()->_uint;
   sendAck( m_data.pos( ) );
   m_data.inc( );
   lDebug( "Pushing data, now at %d", m_data.pos() );
   
   if( m_data.pos() * 4 == sizeof( Tdt::SMmpHeader ) )
   {
      if( m_data.header().dataLength > m_data.maxReceiveSize() )
      {
         lFatal( LDS("POTB", "Block too big: %d"), m_data.header().dataLength );
      }
   }
   
   if( ( m_data.pos() * 4 >= sizeof( Tdt::SMmpHeader ) )
       && ( m_data.pos() * 4
           >= sizeof( Tdt::SMmpHeader ) + m_data.header().dataLength )
       )
   {
      lDebug(LDS("TRFI p=%d", "Transfer finished; pos=%d"), m_data.pos());
      crc32_t crc32=crc32Init( );
      crc32=crc32Update( crc32, m_data.data(), m_data.header().dataLength );
      crc32=crc32Finalize(crc32);
      if( ! m_data.header().dataLength )
      {
         crc32=0xffffffff;
      }
      if( crc32 != m_data.header().crc32Data )
      {
         lWarning( LDS( "CRCWR", "CRC Wrong" ) );
         //": HD 0x%X vs. CL 0x%X",
         //             "CRC32 wrong: header 0x%X vs. calc 0x%X")
         //         , m_data.header().crc32Data, crc32);
         lDebug( "Cl: 0x%X", crc32);
         //dumpMem(m_data.header(), sizeof( Tdt::SMmpHeader ) );
      }
      else
      {
         //handleMmpTransfer( m_data );
         //m_data.m_finished=true;
         return(true);
      }
      //m_data.reset();
   }
   return(false);
}


bool CMmpTransfer::handleTx( const Tdt::CMessage& msg )
{
   lDebug( "   handle Transceiver [%d]", m_nodeId );
   lDebug( "   RCV ACK pos %d %s", msg.getTdtValue()->_uint,
          (msg.getTdtObject() == Tdt::EObject::acknowledgeTransfer)
            ? "TRANSFER" : "SHRED" );
   
   if( msg.getTdtObject() == Tdt::EObject::acknowledgeTransfer )
   {
      // TBD: check for plausibility
      return(true);
   };
   
   if( msg.getTdtObject() != Tdt::EObject::acknowledgeShred )
   {
      lFatal( LDS( "ONSA", "TDT-Object was not a shred acknowledge" ) );
   };
   
   if( msg.getTdtValue()->_int == -1 )
   {
      lCritical( LDS("RSTP", "Resetting position" ) );
      m_data.reset();
      return(false);
   }
   if( msg.getTdtValue()->_int > 0 )
   {
      if( msg.getTdtValue()->_int == m_data.pos()-1 )
      {
         lCritical( LDS( "IOA", "Ignoring old ACK" ) );
         return(false);
      }
      if( msg.getTdtValue()->_int != m_data.pos() )
      {
         lWarning( LDS("ANP", "ACK not plausible") );
         //lWarning("msg %d vs. cur %d", msg.getTdtValue()->_uint
         //         ,m_data.pos() );
         sendAbort();
         return(false);
      }
   }
   m_data.inc();
   if( ( m_data.pos() * sizeof(uint32_t) )
       >= sizeof(Tdt::SMmpHeader) + m_data.header().dataLength )
   {
      // The transmission finished. But still waiting for Transfer Ack.
      // "Reboot" and "Jump to application" wont send an transfer ack.
      if( ( m_data.header().mmpObject == EMmpObject::jumpApplication )
       || ( m_data.header().mmpObject == EMmpObject::reset ) )
      {
         // manipulate the shred Ack to be the transfer ack.
         /*
         msg.setObject( EObject::acknowledgeTransfer );
         msg.setValue( {._uint=0} );
         */
         return( true );
      }
      return( false );
   }
   else
   {
      sendShred();
   }
   return(false);
}


void CMmpTransfer::sendShred()
{
   writeMMP( Tdt::EObject::firmwareDate, m_data.pos(), m_data.data32() );
   //m_timeoutTimer.start( m_shredTimeout );
}


void CMmpTransfer::sendAbort()
{
   writeMMP( Tdt::EObject::firmwareDate, -1, 0 );
   //m_timeoutTimer.stop( );
}


void CMmpTransfer::sendAck( uint32_t pos )
{
   Tdt::CMessage message
       { cbGetNodeId(),
           Tdt::EFunctionCode::ackDataBlob,
           Tdt::EObject::acknowledgeShred,
           Tdt::EUnit::null, { ._uint = pos }
       };
   #if ! defined ( STM32 )
      emit sendMessage( message );
   #else
      // TBD
      cbSendTdtMessage( message );
   #endif
}

void CMmpTransfer::sendTransferAck(int sta)
{
   Tdt::CMessage message
       { cbGetNodeId(),
           Tdt::EFunctionCode::ackDataBlob,
           Tdt::EObject::acknowledgeTransfer,
           Tdt::EUnit::null, { ._uint = sta }
       };
   cbSendTdtMessage( message );
}

/*
void CMmpTransfer::handleMmpTransfer(CMmpTransferData& data)
{
   lInfo("TRNS");
}
*/


}; // namespace Tdt


//---fin----------------------------------------------------------------------
