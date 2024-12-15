#ifndef TDT_MMP_HPP
#define TDT_MMP_HPP
//----------------------------------------------------------------------------
///
/// \brief  Multi-message-package suzpport for TDT protocol
///
///         Transfer BLOBs from one device to another.
/// 
///         Handling the shreds directly (writing them to flash) are making CRC
///         handling impossible. Retransmitting an transfer does not work any 
///         more because the flash would be needed to erased again.
///
/// \date   20241003
/// \author Maximilian Seesslen <mes@seesslen.net>
///
//----------------------------------------------------------------------------


//---Includes-----------------------------------------------------------------


//---Own------------------------------

#include <tdt/message.hpp>
#include <lepto/crc32.h>
#include <stdlib.h>           // malloc
#include <lepto/signal.h>

#if defined( STM32 )
   #define slots
#endif


//---Declaration--------------------------------------------------------------

class CCan;
extern CCan* mmpCan;

namespace Tdt
{

enum class EMmpObject: uint32_t
{
   null,
   log,
   eraseApplicationFlash,
   writeApplicationFlash,
   dumpApplicationFlash,
   //enableApplication,
   //disableApplication,
   jumpApplication,
   reset,
};

struct SMmpHeader
{
   uint32_t magic;
   uint32_t dataLength;
   uint32_t sourceNodeId;
   uint32_t crc32Data;
   EMmpObject mmpObject;
   //uint32_t mmpFunctionCode;
   uint32_t flashAddress;
   uint32_t reserved[ 8 - 6 ];
} __attribute__( ( packed ) );

static_assert( sizeof(SMmpHeader) == 8 * 4 );

class CMmpTransferData
{
   friend class CMmpTransfer;
   private:
      SMmpHeader m_header;
      char* m_data;
      int m_maxReceiveSize;
      int m_retry=0;
      //bool m_finished=false;
      /*
      enum class EState
      {
         init,
         waitAck,
         finished,
      }m_state;
      */
      enum class EDirection
      {
         invalid,
         out,
         in
      }m_direction;
      int m_pos;
      
  public:
      Tdt::EObject m_object=Tdt::EObject::firmwareDate;

   public:
      CMmpTransferData()
      {
         m_header.dataLength=0;
         m_header.mmpObject=EMmpObject::null;
         m_maxReceiveSize=0x0;
         m_data=nullptr;
         reset();
      };
      void alloc()
      {
         m_maxReceiveSize=0x200;
         m_data=(char*)malloc( m_maxReceiveSize );
         reset();
      };
      void updateCrc();
      int pos()
      {
         return(m_pos);
      };
      
      /** \brief  Returns reference to current data
       * 
       *          This can be in the header or in the data blob.
       */
      uint32_t &data32()
      {
         static_assert( sizeof(SMmpHeader) % sizeof(uint32_t) == 0 );
         if( m_pos < sizeof(SMmpHeader) / 4 )
            return( ( (uint32_t*)&m_header ) [ m_pos ] );
         if(!m_data)
         {
            lFatal("No data to send");
         }
         return( ( (uint32_t*)m_data )[ m_pos - ( sizeof(SMmpHeader) / 4 ) ] );
      };
      const char* data() const
      {
         return(m_data);
      }
      void inc()
      {
         m_retry=0;
         m_pos++;
      }
      void reset()
      {
         lDebug("Reset data");
         m_retry=0;
         m_pos=0;
         //m_header.mmpObject=EMmpObject::null;
         //m_finished=false;
      }
      int incRetry()
      {
         m_retry++;
         return(m_retry);
      }
      const SMmpHeader& header() const
      {
         return(m_header);
      }
      int maxReceiveSize()
      {
         return(m_maxReceiveSize);
      }
      void setData(EMmpObject object, const char* data, uint32_t size)
      {
         m_header={
             .magic=0x1234,
             .dataLength=(uint32_t)size,
             .sourceNodeId=0,
             .mmpObject=object,
             .flashAddress=0
             //.mmpFunctionCode=0,
         };
         m_data=(char*)data;
         updateCrc();
         m_maxReceiveSize=0;
         reset();
      }
      void setFlashAddress(address_t addr)
      {
         m_header.flashAddress=addr;
      }
};

class CSocketCan;

class CMmpTransfer
            #if ! defined STM32
            :public QObject
            #endif
{
   #if ! defined STM32
   Q_OBJECT
   #endif
   
   private:
      int m_counterNodeId=0;
       int m_nodeId=0;

   #if ! defined( STM32 )
   signals:
         void sendMessage( const Tdt::CMessage& msg );
   #else
      //public:
      // CSignal< void, const Tdt::CMessage& > sendMessage;
       void cbSendMessage( const Tdt::CMessage& );
       int cbGetNodeId();
   #endif
         
   //signals:
   //      void finishTransfer( CMmpTransferData &data );
      
   public:
      CMmpTransferData m_data;
      //QTimer m_timeoutTimer;
      static constexpr const int m_shredTimeout=250;
      
   public:
      CMmpTransfer();
      virtual ~CMmpTransfer();
       
      void writeMMP( Tdt::EObject object, int pos, uint32_t value );
       
      void startTx()
      {
         m_data.reset();
         writeMMP( m_data.m_object, 0, m_data.data32() );
         //m_timeoutTimer.start( m_shredTimeout );
      }
      
      bool handleRx( const Tdt::CMessage& msg );
      bool handleTx( const Tdt::CMessage& msg );
      
      void sendShred( );
      void sendAbort( );
      void sendAck( uint32_t pos=false );
      void sendTransferAck(int sta);
      
      //void handleMmpTransfer(CMmpTransferData& data);
      /*
      bool finished()
      {
         return( m_data.m_finished );
      }
      */
      void setCounterNodeId( int nodeId )
      {
         m_counterNodeId=nodeId;
      }
};


class CMmpNode
               #if ! defined STM32
               :public QObject
               #endif
{
   #if ! defined STM32
   Q_OBJECT
   #endif
   
   public:
      CMmpTransfer m_tx;
      CMmpTransfer m_rx;
      
   //signals:
      //void send(Tdt::CMessage& msg);
   //signals:
      //void sendMessage( const Tdt::CMessage& msg );
      #if ! defined( STM32 )
         signals:
         void sendMessage( const Tdt::CMessage& msg );
      #elif 0
         CSignal< void, const Tdt::CMessage& > signalSendMessage;
         CSignal< int, const Tdt::CMmpTransferData& > signalHandleMmpTransfer;
      #else
         void cbSendTdtMessage( const Tdt::CMessage& );
         int cbHandleMmpTransfer( const Tdt::CMmpTransferData& );
      #endif
      
   public slots:
         void receive(const CMessage &msg);
      void slotSendMessage( const Tdt::CMessage& msg );
      
   public:
      CMmpNode();
       /*
      bool finishedRx()
      {
         return( m_rx.finished() );
      }
      */
      /*
      void finishTransmit()
      {
         //m_tx.m_data.header().mmpObject = EMmpObject::null;
         m_tx.m_data.reset();
      }
*/
      bool retryTransmit()
      {
         if( m_tx.m_data.incRetry() > 4 )
         {
            lWarning( "Too much retries" );
            return(false);
         }
         lWarning( "Retry pos %d", m_tx.m_data.pos() );
         m_tx.sendShred();
         
         return(true);
      }
      bool transmitActive()
      {
         return( m_tx.m_data.header().mmpObject != EMmpObject::null );
      }
      void startTx(EMmpObject object, const char* data, uint32_t length)
      {
         m_tx.m_data.setData(object, data, length);
         m_tx.startTx();
      }
      void setDestinationNodeId( int nodeId )
      {
         m_tx.setCounterNodeId( nodeId );
      }
      void sendTransfer(Tdt::EMmpObject object, const char *data=0
                     , int length=0, int flashPos=0)
      {
         m_tx.m_data.setData( object, data, length );
         m_tx.m_data.setFlashAddress( flashPos );
         m_tx.startTx( );
      }
};


} // namespace Tdt


//---fin----------------------------------------------------------------------
#endif // ? ! TDT_MMP_HPP
