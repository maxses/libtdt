#ifndef TDT_MMP_NODE_HPP
#define TDT_MMP_NODE_HPP
/**---------------------------------------------------------------------------
 *
 * @file    mmpNode.hpp
 * @brief   Multi-message-package support for TDT protocol
 *
 * Implementation of the MMP protocol of a node. The class contains a transmit
 * and a receive channel.
 * The class can be used on target MCU as well as on host PC. Real functions
 * for sending CAN messages can be linked in via signals (liblepto) or callback
 * functions.
 *
 * Message handling on transmitting side:
 *
 *        -->  receiveTdtMessage()        |----------|
 *        <--  signalSendTdtMessage       | CMmpNode |
 *        <--  signalHandleMmpTransferAck |----------|
 *
 * Message handling on receiving side:
 *
 *        -->  receiveTdtMessage()        |----------|
 *        <--  signalSendTdtMessage       | CMmpNode |
 *        <--  signalHandleMmpTransfer    |----------|
 *
 * @date      20241003
 * @author    Maximilian Seesslen <src@seesslen.net>
 * @copyright SPDX-License-Identifier: Apache-2.0
 *
 *--------------------------------------------------------------------------*/


/*--- Includes -------------------------------------------------------------*/


#include <tdt/mmp.hpp>
#include <tdt/mmpTransfer.hpp>


/*--- Declaration ----------------------------------------------------------*/


#if IS_ENABLED( CONFIG_TDT_MMP_CALLBACKS )
   void cbSendTdtMessage( const Tdt::CMessage& );
   Tdt::EReturnCode cbHandleMmpTransfer( const Tdt::CMmpTransfer& );
   void cbHandleMmpTransferAck( const Tdt::CMmpTransfer& );
#endif

namespace Tdt
{


class CMmpNode
            #if ! defined STM32
               :public QObject
            #endif
{
   #if ! defined STM32
      Q_OBJECT
   #endif

      nodeId_t m_nodeId=0;

      // Needed for unit tests.
      #if ! defined STM32
            QList<Tdt::CMessage> m_rxBuffer;
      #endif

   public:
      CMmpTransfer m_rx;
      CMmpTransfer m_tx;

      QTimer m_rxTimeoutTimer;
      QTimer m_txTimeoutTimer;
      
      // (125/4) is ok when there are only 2 praticipants. (125/2) is needed
      // when more nodes are connected (Tested with 10 devices).
      static constexpr const int m_shredTimeout = 125/1;
      // Commands can take very long time; e.g. erasing a 128KB Flash
      static constexpr const int m_transferExecutionTimeout = 1000*4;
      // Don't keep it too low; testing will simulate frame drops
      static constexpr const int m_receiverTimeout = 1000 * 6;

      #if ! defined STM32
         int m_totalRxTransfers=0;
         int m_messageCounter=0;
         bool m_simulateDrops=false;
      #endif
      
      #if ! defined( STM32 )
      signals:
         void signalSendTdtMessage( const Tdt::CMessage& msg );
         Tdt::EReturnCode signalHandleMmpTransfer( const Tdt::CMmpTransfer& data );
         void signalHandleMmpTransferAck( const Tdt::CMmpTransfer& data );
      #elif IS_ENABLED( CONFIG_TDT_MMP_SIGNALS )
         CSignal< void, const Tdt::CMessage& > signalSendTdtMessage;
         CSignal< Tdt::EReturnCode, const Tdt::CMmpTransfer& > signalHandleMmpTransfer;
         CSignal< void, const Tdt::CMmpTransfer& > signalHandleMmpTransferAck;
      #elif IS_ENABLED( CONFIG_TDT_MMP_CALLBACKS )
         // void cbSendTdtMessage( const Tdt::CMessage& );
         // int cbHandleMmpTransfer( const Tdt::CMmpTransfer& );
         // void cbHandleMmpTransferAck( const Tdt::CMmpTransfer& );
      #else
         #error Either set CONFIG_TDT_MMP_SIGNALS or CONFIG_TDT_MMP_CALLBACKS
      #endif
      
   public slots:
      void receiveTdtMessage( const Tdt::CMessage& msg );
      int dummyHandleMmpTransfer( const Tdt::CMmpTransfer& data );
      //void slotSendTdtMessage( const Tdt::CMessage& msg );
      //int slotHandleMmpTransfer( Tdt::CMmpTransfer& data );
          
   public:
      CMmpNode( nodeId_t nodeId = 2 );
      bool retryTransmit()
      {
         if( m_tx.incRetry() > 4 )
         {
            qWarning( LDS( "TMR", "Too much retries" ) );
            return(false);
         }
         qWarning( LDS( "RTP%d", "Retry pos %d"), m_tx.pos() );
         sendTxShred();
         
         return(true);
      }

      void setCounterNodeId( nodeId_t nodeId )
      {
         m_tx.setCounterNodeId( nodeId );
      }

      void startTransfer(Tdt::EMmpCommand command, const char *data=0
                     , int length=0, int flashPos=0)
      {
         m_tx.setData( command, data, length );
         m_tx.setFlashAddress( flashPos );
         startTx( );
      }

      void setSourceNodeId( nodeId_t id )
      {
         m_tx.setSourceNodeId( id );
      }

      void startTx()
      {
         m_tx.reset();

         #if IS_ENABLED( CONFIG_TDT_PEDANTIC )
            m_tx.setState( ENodeState::transmitting );
         #endif

         sendTxShred( 0, m_tx.data32() );
      }

      void sendTxShred( );
      void sendTxAbort( );
      void sendAck( int pos /*=false*/ );
      void sendTransferAck(int pos, Tdt::EReturnCode sta);
      void sendTxShred( int pos, uint32_t value );
      bool handleRx( const Tdt::CMessage& msg );
      bool handleTx( const Tdt::CMessage& msg );
      nodeId_t getNodeId() const
      {
         return( m_nodeId );
      }
      void setNodeId( nodeId_t nodeId )
      {
         m_nodeId = nodeId;
      }
      public slots:
         void txTimeout();
         void rxTimeout();

      #if ! defined( STM32 )

      public slots:
         void pushRxMessage( const Tdt::CMessage& msg )
         {
            m_rxBuffer+=msg;
         }

      public:
         void testEventLoop()
         {
            while( m_rxBuffer.count() )
            {
               receiveTdtMessage( m_rxBuffer.front() );
               m_rxBuffer.pop_front();
            }
         }

         int getTotalRxTransfers() const
         {
            return( m_totalRxTransfers );
         }
         
         #if IS_ENABLED( CONFIG_TDT_PEDANTIC )

            ENodeState state()
            {
               return( m_rx.state() | m_tx.state() );
            }

         #endif // ? CONFIG_TDT_PEDANTIC
         
      #endif

      void dump() const;
      void emitHandleMmpTransferAck();
      void finishTx( const Tdt::CMessage& msg );
      void finishRx( const Tdt::CMessage& msg );

      #if ! defined( STM32 )
         void doSimulateDrops()
         {
            m_simulateDrops=true;
         }
      #endif
};


} // namespace Tdt


/*--- Fin ------------------------------------------------------------------*/
#endif // ? ! TDT_MMP_NODE_HPP
