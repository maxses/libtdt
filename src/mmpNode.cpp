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


CMmpNode::CMmpNode()
{
   connect( &m_tx, SIGNAL( sendMessage( const Tdt::CMessage& ) ),
           this, SLOT( slotSendMessage( const Tdt::CMessage& ) ) );
   connect( &m_rx, SIGNAL( sendMessage( const Tdt::CMessage& ) ),
           this, SLOT( slotSendMessage( const Tdt::CMessage& ) ) );
}


void CMmpNode::receive(Tdt::CMessage& msg)
{
   qDebug("MMP in");
   switch( msg.getFunctionCode() )
   {
      case ( Tdt::EFunctionCode::dataBlob ):
         qDebug("   MMP Data");
         m_rx.handleRx(msg);
         break;
      case ( Tdt::EFunctionCode::ackDataBlob ):
         qDebug("   MMP Ack");
         m_tx.handleTx(msg);
         break;
      default:
         break;
   }
}


void CMmpNode::slotSendMessage( const Tdt::CMessage& msg )
{
   emit sendMessage( msg );
}


}; // namespace Tdt


//---fin----------------------------------------------------------------------
