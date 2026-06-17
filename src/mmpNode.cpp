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
 * @author    Maximilian Seesslen <mes@seesslen.net>
 * @copyright SPDX-License-Identifier: Apache-2.0
 *
 *--------------------------------------------------------------------------*/


/*--- Includes -------------------------------------------------------------*/


#include <tdt/mmp.hpp>
#if defined ( STM32 )
   //#include <biwak/can.hpp>
#endif


/*--- Implementation -------------------------------------------------------*/


namespace Tdt
{


CMmpNode::CMmpNode()
{
   m_rx.m_data.alloc();

   #if ! defined ( STM32 )
   connect( &m_tx, SIGNAL( sendTdtMessage( const Tdt::CMessage& ) ),
           this, SLOT( slotSendTdtMessage( const Tdt::CMessage& ) ) );
   connect( &m_rx, SIGNAL( sendTdtMessage( const Tdt::CMessage& ) ),
           this, SLOT( slotSendTdtMessage( const Tdt::CMessage& ) ) );
   connect( &m_tx, SIGNAL( handleMmpTransfer( Tdt::CMmpTransferData& ) ),
           this, SLOT( slotHandleMmpTransfer( Tdt::CMmpTransferData& ) ) );
   connect( &m_rx, SIGNAL( handleMmpTransfer( Tdt::CMmpTransferData& ) ),
           this, SLOT( slotHandleMmpTransfer( Tdt::CMmpTransferData& ) ) );
   #elif 0
   m_tx.sendMessage.connect(this, &CMmpNode::slotSendMessage );
   m_rx.sendMessage.connect(this, &CMmpNode::slotSendMessage );
   #else
   #endif
}


void CMmpNode::receiveTdtMessage( const Tdt::CMessage& msg )
{
   qDebug("MMP in");
   switch( msg.getFunctionCode() )
   {
      case ( Tdt::EFunctionCode::dataBlob ):
         qDebug("   MMP Data");
         if ( m_rx.handleRx( msg ) )
         {
            //signalHandleMmpTransfer.emitSignal( m_rx.m_data );
            #if defined STM32
               int sta=cbHandleMmpTransfer( m_rx.m_data );
            #else
               int sta=emit handleMmpTransfer( m_rx.m_data );
            #endif
            m_rx.sendTransferAck(sta);
            m_rx.m_data.reset();
         }
         break;
      case ( Tdt::EFunctionCode::ackDataBlob ):
         qDebug("   MMP Ack");
         m_tx.handleTx( msg );
         break;
      default:
         break;
   }
}


void CMmpNode::slotSendTdtMessage( const Tdt::CMessage& msg )
{
   //lInfo("MMP out");
   #if ! defined ( STM32 )
      emit sendTdtMessage( msg );
   #elif 0
      signalSendMessage.emitSignal( msg );
   #else
      cbSendTdtMessage( msg );
      // TBD
      //mmpCan->send( msg );
   #endif
}

#if ! defined STM32

int CMmpNode::slotHandleMmpTransfer( CMmpTransferData &data )
{
   return( emit handleMmpTransfer( data ) );
}

#endif

}; // namespace Tdt


/*--- Fin ------------------------------------------------------------------*/
