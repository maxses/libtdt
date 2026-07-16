/**---------------------------------------------------------------------------
 *
 * @file    mmpNode.cpp
 * @brief   Multi-message-package support for TDT protocol
 *
 * Transfer BLOBs from one device to another.
 * 
 * Handling the shreds directly (writing them to flash) is making CRC
 * handling impossible. Retransmitting an transfer does not work any 
 * more because the flash would be needed to erased again.
 * 
 * @date      20241003
 * @author    Maximilian Seesslen <src@seesslen.net>
 * @copyright SPDX-License-Identifier: Apache-2.0
 *
 *--------------------------------------------------------------------------*/


/*--- Includes -------------------------------------------------------------*/


#include <tdt/mmpNode.hpp>
#include <tdt/mmpTransfer.hpp>

#if defined ( STM32 )
   //#include <biwak/can.hpp>
#endif

#if ( ! defined STM32 ) && defined ( USE_LEPTO )
   #include <lepto/print.h>      // hexDump
#endif

#if ( ! defined STM32 )
   #include <QRandomGenerator>
   #include <QThread>
#endif


/*--- Implementation -------------------------------------------------------*/


namespace Tdt
{


CMmpNode::CMmpNode( nodeId_t nodeId )
         :m_nodeId( nodeId )
         ,m_rx( CMmpTransfer::EDirection::in )
         ,m_tx( CMmpTransfer::EDirection::out )
{
   m_rx.allocRx();

   #if ! defined ( STM32 )
      connect( &m_txTimeoutTimer, SIGNAL( timeout() ),
              this, SLOT( txTimeout() ) );
      connect( &m_rxTimeoutTimer, SIGNAL( timeout() ),
              this, SLOT( rxTimeout() ) );
   #else
      m_txTimeoutTimer.timeout.connect( this, &CMmpNode::txTimeout );
      m_rxTimeoutTimer.timeout.connect( this, &CMmpNode::rxTimeout );
   #endif
}


void CMmpNode::receiveTdtMessage( const Tdt::CMessage& msg )
{
   // Expand unit tests: Loose messages
   #if ! defined STM32
      m_messageCounter++;
   #endif

   #if ! defined STM32
   // qDebug("MMP in");
   const char *fc="Unknown";
   if( msg.getFunctionCode() == Tdt::EFunctionCode::dataBlob )
   {
      fc="dataBlob";
   }
   if( msg.getFunctionCode() == Tdt::EFunctionCode::ackDataBlob )
   {
      fc="ackData";
   }
   /*
   if( msg.getFunctionCode() == Tdt::EFunctionCode::ackTransfer )
   {
      fc="ackTransfer";
   }
   */
   
   (void)fc;
   #endif

   //qDebug("      IN #%d: FC=%s POS=%d", m_nodeId, fc, msg.getMmpPos());

   // Expand unit tests: Loose messages
   #if ! defined STM32
      if( m_simulateDrops )
      {
         // "Loose" messsages. Don't drop too much, timeouts will kick in
         if( ! ( m_messageCounter % ( 24 + ( QRandomGenerator::global()->generate() % 24 ) ) ) )
         {
            qDebug("         Dropping");
            return;
         }
      }
   #endif

   switch( msg.getFunctionCode() )
   {
      case ( Tdt::EFunctionCode::dataBlob ):
         handleRx( msg );
         break;
      case ( Tdt::EFunctionCode::ackDataBlob ):
         handleTx( msg );
         break;
      #if 0
      case ( Tdt::EFunctionCode::ackTransfer ):
         finishTx( msg );
         break;
      #endif
      default:
         break;
   }
}


void CMmpNode::sendTxShred( int pos, uint32_t value )
{
   Tdt::CMessage message( m_tx.getCounterNodeId(),
                         Tdt::EFunctionCode::dataBlob,
                         m_nodeId, // TBD: Its me, the source
                         pos, value );
   #if ! defined ( STM32 )
      emit signalSendTdtMessage( message );
   #elif IS_ENABLED( CONFIG_TDT_MMP_SIGNALS )
      signalSendTdtMessage.emitSignal( message );
   #elif IS_ENABLED( CONFIG_TDT_MMP_CALLBACKS )
      cbSendTdtMessage( message );
   #else
      #error "You have to set either CONFIG_TDT_MMP_SIGNALS or CONFIG_TDT_MMP_CALLBACKS"
   #endif

   if( pos >= 0 )
   {
      m_txTimeoutTimer.start( m_tx.isLast(pos) ? m_transferExecutionTimeout : m_shredTimeout * 1 );
   }
}


bool CMmpNode::handleRx( const Tdt::CMessage& msg )
{
   // Indicator to abort ttransfer
   if( msg.getMmpPos() == -1 )
   {
      qCritical( LDS( "MMAB", "MMP abort" ) );
      m_rx.reset();
      #if IS_ENABLED( CONFIG_TDT_PEDANTIC )
         m_rx.setState( ENodeState::idle );
      #endif
      return( false );
   }

   // An retransmission of an messqage i already have: still acknowledge
   if( m_rx.pos() && ( msg.getMmpPos() == m_rx.pos() - 1 ) )
   {
      qCritical( LDS("IGRE", "Ignoring retransmit" ) );

      // Cornercase: Sender may have lost return value. Just send the status again
      if( m_rx.isLast( msg.getMmpPos() ) )
      {
         sendTransferAck( msg.getMmpPos(), m_rx.returnCode() );
      }
      else
      {
         sendAck( msg.getMmpPos() );
      }
      return( false );
   }

   // Retransmission of the last shred when receiver has already finished.
   // Assume the header is still valid.
   if( ( m_rx.pos() == 0 ) && msg.getMmpPos() )
   {
      #if IS_ENABLED( CONFIG_TDT_PEDANTIC )
         if( m_rx.state() != ENodeState::idle )
         {
            qFatal("Foo");
         }
      #endif

      if( m_rx.isLast( msg.getMmpPos() ) )
      {
         //sendAck( msg.getMmpPos() );
         sendTransferAck( msg.getMmpPos(), m_rx.returnCode() );
      }
      else
      {
         #if IS_ENABLED( CONFIG_TDT_PEDANTIC )
         int msgPos=msg.getMmpPos();
         int size=(int)sizeof( Tdt::SMmpHeader ) + m_rx.header().dataLength;
         
         qFatal("Does not make sense: pos %d of total %d is not last?", msgPos, size);
         #else
            qFatal("UP");
         #endif
      }
      return( false );
   }

   if( msg.getMmpPos() != m_rx.pos() )
   {
      qCritical( LDS("M ODNM m %d vs c %d", "MMP order missmatch: message %d vs. buffer %d" )
                , (int)msg.getMmpPos(), (int)m_rx.pos() );

      m_rx.reset();
      #if IS_ENABLED( CONFIG_TDT_PEDANTIC )
         m_rx.setState( ENodeState::idle );
      #endif
      sendAck( -1 );
      return( false );
   }

   if( !m_rx.pos() )
   {
      #if IS_ENABLED( CONFIG_TDT_PEDANTIC )
         if( m_rx.state( ) != ENodeState::idle )
         {
            qFatal( LDS("RSO", "Reseiving still ongoing") );
         }
      #endif
      m_rx.setCounterNodeId( msg.getNodeId() );
   }

   if( m_rx.getCounterNodeId( ) != msg.getNodeId() )
   {
      qFatal( LDS("SMM", "Sender missmatch") );
   }

   m_rx.data32()=msg.getTdtValue()->_uint;

   if( ! m_rx.isLast() )
   {
      sendAck( m_rx.pos( ) );
   }

   m_rx.inc( );

   #if 0
      qDebug( "Pushing received data, now at %d", m_rx.pos() );
   #endif

   // Have i just finished receiving the header? Make some checks.
   // Maybe allocate the memory.
   if( m_rx.pos() * 4 == sizeof( Tdt::SMmpHeader ) )
   {
      if( m_rx.header().dataLength > m_rx.maxReceiveSize() )
      {
         qFatal( LDS("POTB %d", "Block too big: %d"), m_rx.header().dataLength );
      }
   }

   if( m_rx.isFinished() )
   {
      finishRx( msg );
   }
   return(false);
}


bool CMmpNode::handleTx( const Tdt::CMessage& msg )
{
   if( msg.getNodeId() != m_tx.getCounterNodeId() )
   {
      // Just some ack, not for me
      qDebug("[TX] Error: Wrong counter node id: Expected %d, message has %d", m_tx.getCounterNodeId(), msg.getNodeId());

      return(false);
   }
   m_txTimeoutTimer.stop();
   
   int pos=msg.getMmpPos();

   if( pos == -1 )
   {
      qCritical( LDS("RSTP", "Resetting position" ) );
      m_tx.reset();
      #if IS_ENABLED( CONFIG_TDT_PEDANTIC )
         m_tx.setState( ENodeState::idle );
      #endif
      return(false);
   }
   
   if( pos > 0 )
   {
      if( pos == m_tx.pos()-1 )
      {
         // An STM32F103 in the bus forced an STM32L4 to unnecessary retransmits.
         // This could also be seen in cordyceps by scanning devices.
         qWarning( LDS( "IOA %d", "Ignoring old/previous ACK; MSG:%d" ),
                  pos );
         return(false);
      }
      if( pos != m_tx.pos() )
      {
         qWarning( LDS("ANP", "ACK not plausible") );
         #if ! defined ( STM32 )
            qWarning("msg %d vs. cur %d", msg.getTdtValue()->_uint
                  ,m_tx.pos() );
         #endif
         sendTxAbort();
         return(false);
      }
   }
   
   m_tx.inc();
   if( m_tx.isFinished() )
   {
      // This can happen when the last shred also has an ACK and the Tranferack
      // follows after that.
      // m_tx.dec();
      // qFatal("This should not happen");
      // The whole transmission has finished; THis is the ack for the last shred
      finishTx( msg );
      return( false );
   }
   else
   {
      sendTxShred();
   }
   return(false);
}


void CMmpNode::sendAck( int pos )
{
   Tdt::CMessage message
   {
       m_nodeId,
      Tdt::EFunctionCode::ackDataBlob,
      (uint16_t)m_rx.getCounterNodeId(),
      (int16_t)pos,
      0
   };

   m_rxTimeoutTimer.start( m_receiverTimeout  );

   #if ! defined ( STM32 )
      emit signalSendTdtMessage( message );
   #else
      #if IS_ENABLED( CONFIG_TDT_MMP_SIGNALS )
         signalSendTdtMessage.emitSignal( message );
      #elif IS_ENABLED( CONFIG_TDT_MMP_CALLBACKS )
         cbSendTdtMessage( message );
      #else
         #error "Set either CONFIG_TDT_MMP_SIGNALS or CONFIG_TDT_MMP_CALLBACKS"
      #endif
   #endif
}


void CMmpNode::sendTransferAck(int pos, Tdt::EReturnCode sta)
{
   Tdt::CMessage message
   {
      m_nodeId,
      //Tdt::EFunctionCode::ackTransfer,
      Tdt::EFunctionCode::ackDataBlob,
      (uint16_t)m_rx.getCounterNodeId(),
      (int16_t)pos,
      (uint32_t)sta
   };

   #if ! defined ( STM32 )
      emit signalSendTdtMessage( message );
   #else
      #if IS_ENABLED( CONFIG_TDT_MMP_SIGNALS )
         signalSendTdtMessage.emitSignal( message );
      #elif IS_ENABLED( CONFIG_TDT_MMP_CALLBACKS )
         cbSendTdtMessage( message );
      #else
         #error "Set either CONFIG_TDT_MMP_SIGNALS or CONFIG_TDT_MMP_CALLBACKS"
      #endif
   #endif

   m_rxTimeoutTimer.stop();
}

void CMmpNode::sendTxShred()
{
   sendTxShred( m_tx.pos(), m_tx.data32() );
   //m_timeoutTimer.start( m_shredTimeout );
}

void CMmpNode::sendTxAbort()
{
   sendTxShred( -1, 0 );
}

#if ! defined STM32

int CMmpNode::dummyHandleMmpTransfer( const Tdt::CMmpTransfer& data )
{
   printf( "Goal! ID=%d Pos=%d\n", m_nodeId, data.pos() );

   // Simulate writing to flash: 1/3 of timeout This does not work well. There
   // are messages in the buffers but the whole system sleeps. Afterwards shreds
   // are handled twice.
   // QThread::msleep( m_transferExecutionTimeout / 3 );
   m_totalRxTransfers++;

   // Just store the node-id as transfer status
   return( m_nodeId );
}

#endif

void CMmpNode::txTimeout()
{
   qWarning( LDS("%d TXTO","[%d] TX Timeout"), m_nodeId );

   if( !retryTransmit()  )
   {
      qWarning("CPD");
      m_tx.setReturnCode( EReturnCode::counterPartDead );
      emitHandleMmpTransferAck();
      return;
   }
}

void CMmpNode::rxTimeout()
{
   qWarning( LDS("%d RXTO", "[%d] RX Timeout"), m_nodeId );
   // Don't do any retransmit on the receivers side.
   // Its up to the transmitter to retransmit its data when he got no 
   // acknowledge.
   // This timeout is quite long time and shall completely abort the transfer.
   m_rx.reset();
}

void CMmpNode::dump() const
{
   qDebug("###### Dump #######");
   qDebug("Tx-Timeout: %d / %s / %d", m_txTimeoutTimer.interval(), m_txTimeoutTimer.isActive() ? "Activce": "Inactive"
            ,m_txTimeoutTimer.isSingleShot());
}


void CMmpNode::emitHandleMmpTransferAck()
{
   #if defined STM32
      #if IS_ENABLED( CONFIG_TDT_MMP_SIGNALS )
         signalHandleMmpTransferAck.emitSignal( m_tx );
      #elif IS_ENABLED( CONFIG_TDT_MMP_CALLBACKS )
          cbHandleMmpTransferAck( m_tx );
      #else
         #error "Set either CONFIG_TDT_MMP_SIGNALS or CONFIG_TDT_MMP_CALLBACKS"
      #endif
   #else
      emit signalHandleMmpTransferAck( m_tx );
   #endif

   return;
}


void CMmpNode::finishTx( const Tdt::CMessage& msg )
{
   // Acknowledge is missing but i already got transfer ack ?
   if( ! m_tx.isFinished( m_tx.pos()+1 ) )
   {
      qFatal( LDS( "TACK NFIT %d/%d"
            , "Transfer Ack on non finished ttransfer. Pos is %d, data size is %d.")
            , m_tx.pos(), m_tx.header().dataLength);
   }
   assert( msg.getLen()>= 8 );
   m_tx.setReturnCode( (Tdt::EReturnCode)msg.getTdtValue()->_uint );
   
   #if IS_ENABLED( CONFIG_TDT_PEDANTIC )
      m_tx.setState( ENodeState::idle );
   #endif
   
   m_txTimeoutTimer.stop();
   
   // Handling mmp transfer ack could setup another transfer
   emitHandleMmpTransferAck();
}


void CMmpNode::finishRx( const Tdt::CMessage& msg )
{
   #if defined ( USE_LEPTO )
   
   crc32_t crc32=crc32Init( );
   crc32=crc32Update( crc32, m_rx.data(), m_rx.header().dataLength );
   crc32=crc32Finalize(crc32);
   
   if( ! m_rx.header().dataLength )
   {
      crc32=0xffffffff;
   }
   
   #else
   
   uint32_t crc32=0;
   
   #endif
   
   if( crc32 != m_rx.header().crc32Data )
   {
      qWarning( LDS( "CRCWR", "CRC Wrong" ) );
      m_rx.setReturnCode( EReturnCode::wrongCRC);
   }
   else
   {
      #if defined STM32
         #if IS_ENABLED( CONFIG_TDT_MMP_SIGNALS )
            m_rx.setReturnCode( signalHandleMmpTransfer.emitSingle( m_rx ) );
         #elif IS_ENABLED( CONFIG_TDT_MMP_CALLBACKS )
            m_rx.setReturnCode( cbHandleMmpTransfer( m_rx ) );
         #else
            #error "Set either CONFIG_TDT_MMP_SIGNALS or CONFIG_TDT_MMP_CALLBACKS"
         #endif
      #else
         m_rx.setReturnCode( emit signalHandleMmpTransfer( m_rx ) );
      #endif
   }
   sendTransferAck( msg.getMmpPos(), m_rx.returnCode() );
   m_rxTimeoutTimer.stop();
   m_rx.reset();

   #if IS_ENABLED( CONFIG_TDT_PEDANTIC )
      m_rx.setState( ENodeState::idle );
   #endif
}


#if ! defined STM32

#if 0

int CMmpNode::slotHandleMmpTransfer( CMmpTransfer &data )
{
   return( emit handleMmpTransfer( data ) );
}

#endif

#endif

}; // namespace Tdt

#if IS_ENABLED( CONFIG_TDT_MMP_CALLBACKS )

__attribute__((weak)) void cbSendTdtMessage( const Tdt::CMessage& )
{
   return;
}

__attribute__((weak)) Tdt::EReturnCode cbHandleMmpTransfer( const Tdt::CMmpTransfer& )
{
   return( Tdt::EReturnCode::notImplemented );
};

__attribute__((weak)) void cbHandleMmpTransferAck( const Tdt::CMmpTransfer& )
{
   // Nothing to do, job is done
}

#endif // ? CONFIG_TDT_MMP_CALLBACKS


/*--- Fin ------------------------------------------------------------------*/
