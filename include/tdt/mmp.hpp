#ifndef TDT_MMP_HPP
#define TDT_MMP_HPP
/**---------------------------------------------------------------------------
 *
 * @file    mmp.hpp
 * @brief   Multi-message-package support for TDT protocol
 *
 * Transfer BLOBs from one device to another.
 * 
 * Handling the shreds directly (writing them to flash) are making CRC
 * handling impossible. Retransmitting an transfer does not work any 
 * more because the flash would be needed to erased again.
 * 
 * @date      20241003
 * @author    Maximilian Seesslen <mes@seesslen.net>
 * @copyright SPDX-License-Identifier: Apache-2.0
 *
 *--------------------------------------------------------------------------*/


/*--- Includes -------------------------------------------------------------*/


#include <tdt/message.hpp>
#include <stdlib.h>           // malloc

#if defined USE_LEPTO
   #include <lepto/crc32.h>
   #include <lepto/signal.hpp>
   #if defined( STM32 )
      #include <lepto/mockQt.hpp>
   #endif
#else
   typedef uint32_t address_t;
#endif

#if defined( STM32 )
   #define slots
#else
   #include <QObject>
#endif


/*--- Declaration ----------------------------------------------------------*/


class CCan;
extern CCan* mmpCan;

namespace Tdt
{
   class CMmpTransferData;
}

#if defined( STM32 )
   void cbSendTdtMessage( const Tdt::CMessage& );
   Tdt::nodeId_t cbGetNodeId();
   int cbHandleMmpTransfer( const Tdt::CMmpTransferData& );
#endif

namespace Tdt
{

enum class EMmpCommand: uint32_t
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
   sendEeprom,
   sendFirmware,
   receiveEeprom,
   receiveFirmware,
   writeEeprom,
   dummyCommand,

   eraseBootloaderFlash,
   writeBootloaderFlash,
};

struct SMmpHeader
{
   uint32_t magic;
   int32_t  dataLength;
   uint32_t sourceNodeId;
   uint32_t crc32Data;
   EMmpCommand mmpCommand;
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
      Tdt::EObject m_object=Tdt::EObject::nodeFirmwareDate;

   public:
      CMmpTransferData()
      {
         m_header.dataLength=0;
         m_header.mmpCommand=EMmpCommand::null;
         m_maxReceiveSize=0x0;
         m_data=nullptr;
         reset();
      };
      void alloc()
      {
         #if ! defined STM32
         m_maxReceiveSize=0x10000;
         #else
         m_maxReceiveSize=0x200;
         #endif
         m_data=(char*)malloc( m_maxReceiveSize );
         reset();
      };
      void updateCrc();
      int pos()
      {
         return(m_pos);
      };
      bool dataLeft()
      {
         return( m_pos < ( ( (int)sizeof(SMmpHeader) + m_header.dataLength ) / 4 ) );
      }
      
      /** \brief  Returns reference to current data
       * 
       *          This can be in the header or in the data blob.
       */
      uint32_t &data32()
      {
         static_assert( sizeof(SMmpHeader) % sizeof(uint32_t) == 0 );
         if( m_pos < (int)sizeof(SMmpHeader) / 4 )
            return( ( (uint32_t*)&m_header ) [ m_pos ] );
         if(!m_data)
         {
            qFatal("NDTS");
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
         qDebug("Reset data");
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
      void setData(EMmpCommand command, const char* data, uint32_t size)
      {
         m_header={
             .magic=0x1234,
             .dataLength=(int32_t)size,
             .sourceNodeId=0,
             .mmpCommand=command,
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
      void setSourceNodeId( nodeId_t nodeId )
      {
         m_header.sourceNodeId = nodeId;
      }
};


/**
 * @brief Ongoing mmp transfer in one direction
 * 
 * A node will nedd a rx transfer object and a tx transfer object.
 */
class CMmpTransfer
            #if ! defined STM32
            :public QObject
            #endif
{
   #if ! defined STM32
   Q_OBJECT
   #endif
   
   private:
      nodeId_t m_counterNodeId=0;
      nodeId_t m_nodeId=0;

   #if ! defined( STM32 )
   signals:
         void sendTdtMessage( const Tdt::CMessage& msg );
         int handleMmpTransfer( Tdt::CMmpTransferData& data );
         //uint32_t getNodeId();
   #else
      //public:
      // CSignal< void, const Tdt::CMessage& > sendMessage;
      // void cbSendMessage( const Tdt::CMessage& );
      // nodeId_t cbGetNodeId();
   #endif
         
   //signals:
   //      void finishTransfer( CMmpTransferData &data );
      
   public:
      CMmpTransferData m_data;
      //QTimer m_timeoutTimer;
      static constexpr const int m_shredTimeout=250;
      
   public:
      CMmpTransfer();
      ~CMmpTransfer();
       
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
      void setNodeId( int nodeId )
      {
         m_nodeId=nodeId;
      }
      nodeId_t getTargetNodeId( )
      {
         return( m_counterNodeId );
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
      
      #if ! defined( STM32 )
         signals:
         void sendTdtMessage( const Tdt::CMessage& msg );
         int handleMmpTransfer( Tdt::CMmpTransferData& data );
      #elif 0
         CSignal< void, const Tdt::CMessage& > signalSendMessage;
         CSignal< int, const Tdt::CMmpTransferData& > signalHandleMmpTransfer;
      #else
         // void cbSendTdtMessage( const Tdt::CMessage& );
         // int cbHandleMmpTransfer( const Tdt::CMmpTransferData& );
      #endif
      
   public slots:
      void receive(const CMessage& msg);
      void slotSendTdtMessage( const Tdt::CMessage& msg );
      int slotHandleMmpTransfer( Tdt::CMmpTransferData& data );
          
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
            qWarning( LDS( "TMR", "Too much retries" ) );
            return(false);
         }
         qWarning( LDS( "RTP%d", "Retry pos %d"), m_tx.m_data.pos() );
         m_tx.sendShred();
         
         return(true);
      }
      bool transmitActive()
      {
         return( m_tx.m_data.header().mmpCommand != EMmpCommand::null );
      }
      /*
      void startTx(EMmpCommand command, const char* data, uint32_t length)
      {
         m_tx.m_data.setData(command, data, length);
         m_tx.startTx();
      }
      */
      void setTargetNodeId( nodeId_t nodeId )
      {
         m_tx.setCounterNodeId( nodeId );
      }
      void startTransfer(Tdt::EMmpCommand command, const char *data=0
                     , int length=0, int flashPos=0)
      {
         m_tx.m_data.setData( command, data, length );
         m_tx.m_data.setFlashAddress( flashPos );
         m_tx.startTx( );
      }
      void setSourceNodeId( nodeId_t id )
      {
         m_tx.m_data.setSourceNodeId( id );
      }
};


} // namespace Tdt


/*--- Fin ------------------------------------------------------------------*/
#endif // ? ! TDT_MMP_HPP
