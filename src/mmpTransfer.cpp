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
#if ( ! defined STM32 ) && defined ( USE_LEPTO )
   #include <lepto/print.h>      // hexDump
#endif


//---Implementation-----------------------------------------------------------


namespace Tdt
{


CMmpTransfer::CMmpTransfer()
{
   m_nodeId=0x70;
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
      #if defined USE_LEPTO
         m_header.crc32Data = crc32Init( );
         m_header.crc32Data = crc32Update( m_header.crc32Data
                                          , m_data, m_header.dataLength );
         m_header.crc32Data = crc32Finalize( m_header.crc32Data );
      #else
      m_header.crc32Data = 0;
      #endif
   }
};


void CMmpTransfer::writeMMP( Tdt::EObject object, int pos, uint32_t value )
{
   //if( pos % 100 == 0 )
   {
      qDebug("MMP out: pos %d; length=%d", pos, m_data.header().dataLength);
   }
   Tdt::CMessage message( m_counterNodeId,
                         Tdt::EFunctionCode::dataBlob,
                         (uint16_t)object, // TBD: Its me, the source
                         pos, value );
   //m_socketCan << message;
   #if ! defined ( STM32 )
      emit sendTdtMessage(message);
   #elif 0
      // TBD
      sendMessage.emitSignal( message );
   #else
      cbSendTdtMessage( message );
   #endif
}


bool CMmpTransfer::handleRx( const Tdt::CMessage& msg )
{
   qDebug( "   handle Receiver [%d]", m_nodeId );
   // qDebug("pos %d dlen:%d", (int)msg.getMmpPos(), m_data.header().dataLength);
   if( m_data.pos() == -1 )
   {
      qCritical( LDS( "MMAB", "MMP abort" ) );
      m_data.reset();
      return( false );
   }
   if( m_data.pos() && ( msg.getMmpPos() == m_data.pos() - 1 ) )
   {
      qCritical( LDS("IGRE", "Ignoring retransmit" ) );
      sendAck( m_data.pos()-1 );
      return( false );
   }
   else if( msg.getMmpPos() != m_data.pos() )
   {
      qCritical( LDS("M ODNM", "MMP order does not match:" ) );
      qDebug( "   msg %d vs. cur %d"
                , (int)msg.getMmpPos(), (int)m_data.pos() );

      m_data.reset();
      sendAck( -1 );
      return( false );
   }
   
   qDebug("Pushing data");
   m_data.data32()=msg.getTdtValue()->_uint;
   sendAck( m_data.pos( ) );
   m_data.inc( );
   qDebug( "Pushing data, now at %d", m_data.pos() );
   
   if( m_data.pos() * 4 == sizeof( Tdt::SMmpHeader ) )
   {
      if( m_data.header().dataLength > m_data.maxReceiveSize() )
      {
         qFatal( LDS("POTB", "Block too big: %d"), m_data.header().dataLength );
      }
   }
   
   if( ( m_data.pos() * 4 >= (int)sizeof( Tdt::SMmpHeader ) )
       && ( m_data.pos() * 4
           >= (int)sizeof( Tdt::SMmpHeader ) + m_data.header().dataLength )
       )
   {
      qDebug(LDS("TRFI p=%d", "Transfer finished; pos=%d"), m_data.pos());
      qDebug(LDS(" dl=%d", "   dataLength=%d"), m_data.header().dataLength );

      #if defined ( USE_LEPTO )
      crc32_t crc32=crc32Init( );
      crc32=crc32Update( crc32, m_data.data(), m_data.header().dataLength );
      crc32=crc32Finalize(crc32);
      if( ! m_data.header().dataLength )
      {
         crc32=0xffffffff;
      }
      if( crc32 != m_data.header().crc32Data )
      {
         qWarning( LDS( "CRCWR", "CRC Wrong" ) );
         //": HD 0x%X vs. CL 0x%X",
         //             "CRC32 wrong: header 0x%X vs. calc 0x%X")
         //         , m_data.header().crc32Data, crc32);
         #if ! defined STM32
            qDebug( "   Calck: 0x%X", crc32 );
            qDebug( "   Header: 0x%X", m_data.header().crc32Data );
            //dumpMem(m_data.header(), sizeof( Tdt::SMmpHeader ) );
            hexDump( m_data.data(), m_data.header().dataLength );
         #endif
      }
      else
      #endif // ? USE_LEPTO
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
   if( msg.getNodeId() != m_counterNodeId )
   {
      // Just some ack, not for me
      return(false);
   }
   
   qDebug( "   handle Transceiver [%d]", m_nodeId );
   qDebug( "   RCV ACK pos %d %s", msg.getTdtValue()->_uint,
          (msg.getTdtObject() == Tdt::EObject::mmpAcknowledgeTransfer)
            ? "TRANSFER" : "SHRED" );
   
   if( msg.getTdtObject() == Tdt::EObject::mmpAcknowledgeTransfer )
   {
      // TBD: check for plausibility
      return(true);
   };
   
   if( msg.getTdtObject() != Tdt::EObject::mmpAcknowledgeShred )
   {
      qDebug( "Got: 0x%X Exp: 0x%X", (unsigned int)msg.getTdtObject()
              , (unsigned int)Tdt::EObject::mmpAcknowledgeShred );
      qFatal( LDS( "ONSA", "TDT-Object was not a shred acknowledge" ) );
   };
   
   if( msg.getTdtValue()->_int == -1 )
   {
      qCritical( LDS("RSTP", "Resetting position" ) );
      m_data.reset();
      return(false);
   }
   if( msg.getTdtValue()->_int > 0 )
   {
      if( msg.getTdtValue()->_int == m_data.pos()-1 )
      {
         // An STM32F103 in the bus forced an STM32L4 to unnecessary retransmits.
         // This could also be seen in cordyceps by scanning devices.
         qCritical( LDS( "IOA", "Ignoring old/previous ACK; MSG:%d" ),
                  msg.getTdtValue()->_int );
         return(false);
      }
      if( msg.getTdtValue()->_int != m_data.pos() )
      {
         qWarning( LDS("ANP", "ACK not plausible") );
         //qWarning("msg %d vs. cur %d", msg.getTdtValue()->_uint
         //         ,m_data.pos() );
         sendAbort();
         return(false);
      }
   }
   m_data.inc();
   if( ! m_data.dataLeft() )
   {
      // The transmission finished. But still waiting for Transfer Ack.
      // "Reboot" and "Jump to application" wont send an transfer ack.
      if( ( m_data.header().mmpCommand == EMmpCommand::jumpApplication )
       || ( m_data.header().mmpCommand == EMmpCommand::reset ) )
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
   writeMMP( Tdt::EObject::nodeFirmwareDate, m_data.pos(), m_data.data32() );
   //m_timeoutTimer.start( m_shredTimeout );
}


void CMmpTransfer::sendAbort()
{
   writeMMP( Tdt::EObject::nodeFirmwareDate, -1, 0 );
   //m_timeoutTimer.stop( );
}


void CMmpTransfer::sendAck( uint32_t pos )
{
   Tdt::CMessage message
   {
      #if defined STM32
      cbGetNodeId(),
      #else
      //emit getNodeId(),
       m_nodeId,
      #endif
      Tdt::EFunctionCode::ackDataBlob,
      Tdt::EObject::mmpAcknowledgeShred,
      Tdt::EUnit::null,
      { ._uint = pos }
   };
   #if ! defined ( STM32 )
      emit sendTdtMessage( message );
   #else
      // TBD
      cbSendTdtMessage( message );
   #endif
}

void CMmpTransfer::sendTransferAck(int sta)
{
   Tdt::CMessage message
   {
      #if defined STM32
      cbGetNodeId(),
      #else
      // emit getNodeId(),
      m_nodeId,
      #endif
      Tdt::EFunctionCode::ackDataBlob,
      Tdt::EObject::mmpAcknowledgeTransfer,
      Tdt::EUnit::null,
      { ._int = sta }
   };
   #if ! defined ( STM32 )
      sendTdtMessage( message );
   #else
      cbSendTdtMessage( message );
   #endif
}

/*
void CMmpTransfer::handleMmpTransfer(CMmpTransferData& data)
{
   lInfo("TRNS");
}
*/


}; // namespace Tdt


//---fin----------------------------------------------------------------------
