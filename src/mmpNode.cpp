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
#if defined ( STM32 )
   //#include <biwak/can.hpp>
#endif


//---Implementation-----------------------------------------------------------


namespace Tdt
{


CMmpNode::CMmpNode()
{
   m_rx.m_data.alloc();

   #if ! defined ( STM32 )
   connect( &m_tx, SIGNAL( sendMessage( const Tdt::CMessage& ) ),
           this, SLOT( slotSendMessage( const Tdt::CMessage& ) ) );
   connect( &m_rx, SIGNAL( sendMessage( const Tdt::CMessage& ) ),
           this, SLOT( slotSendMessage( const Tdt::CMessage& ) ) );
   #elif 0
   m_tx.sendMessage.connect(this, &CMmpNode::slotSendMessage );
   m_rx.sendMessage.connect(this, &CMmpNode::slotSendMessage );
   #else
   #endif
}


void CMmpNode::receive( const Tdt::CMessage& msg )
{
   //lInfo("MMP in");
   switch( msg.getFunctionCode() )
   {
      case ( Tdt::EFunctionCode::dataBlob ):
         lDebug("   MMP Data");
         if ( m_rx.handleRx( msg ) )
         {
            //signalHandleMmpTransfer.emitSignal( m_rx.m_data );
            int sta=cbHandleMmpTransfer( m_rx.m_data );
            m_rx.sendTransferAck(sta);
            m_rx.m_data.reset();
         }
         break;
      case ( Tdt::EFunctionCode::ackDataBlob ):
         lDebug("   MMP Ack");
         m_tx.handleTx( msg );
         break;
      default:
         break;
   }
}


void CMmpNode::slotSendMessage( const Tdt::CMessage& msg )
{
   //lInfo("MMP out");
   #if ! defined ( STM32 )
      emit sendMessage( msg );
   #elif 0
      signalSendMessage.emitSignal( msg );
   #else
      cbSendTdtMessage( msg );
      // TBD
      //mmpCan->send( msg );
   #endif
}


}; // namespace Tdt


//---fin----------------------------------------------------------------------
