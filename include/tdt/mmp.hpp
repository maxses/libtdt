#ifndef TDT_MMP_HPP
#define TDT_MMP_HPP
//----------------------------------------------------------------------------
///
/// \brief  Multi-message-package suzpport for TDT protocol
///
///         Transfer BLOBs from one device to another.
///
/// \date   20241003
/// \author Maximilian Seesslen <mes@seesslen.net>
///
//----------------------------------------------------------------------------


//---Includes-----------------------------------------------------------------


//---Own------------------------------

#include <lepto/can_message_tdt.h>
#include <lepto/crc32.h>

#include <QTimer>


//---Forward declarations-----------------------------------------------------


//---Declaration--------------------------------------------------------------


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
      int m_maxTransferSize;
      int m_retry=0;
      bool m_finished=false;
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
         m_maxTransferSize=0x200;
         m_data=(char*)malloc( m_maxTransferSize );
         reset();
      };
      CMmpTransferData(EMmpObject object, const char* data, int size)
         :m_header{
               .magic=0x1234,
               .dataLength=(uint32_t)size,
               .sourceNodeId=0,
               .mmpObject=object,
               //.mmpFunctionCode=0,
           }
         ,m_data( (char*)data )
         //,m_state( EState::init )
      {
         updateCrc();
         m_maxTransferSize=0;
         reset();
      }
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
         m_finished=false;
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
         return(m_maxTransferSize);
      }
      void setData(const char* data, uint32_t size)
      {
         m_data=(char*)data;
         m_header.dataLength=size;
         updateCrc();
         reset();
      }
      void setFlashAddress(address_t addr)
      {
         m_header.flashAddress=addr;
      }
};

class CSocketCan;

class CMmpTransfer   :// public CMmpTransferData, 
                      public QObject
{
   Q_OBJECT
   
   private:
      int m_counterNodeId=0;
       int m_nodeId=0;
      //CSocketCan& m_socketCan;
      
   //Q_SIGNALS:
   //signals:
   public:
         QList<Tdt::CMessage> m_txList;
         void send( Tdt::CMessage& msg )
         {
            m_txList << msg;
         }
   signals:
         void finishTransfer( CMmpTransferData &data );
      
   public:
      CMmpTransferData m_data;
       QTimer m_timeoutTimer;
       static constexpr const int m_shredTimeout=250;
      
   public:
      CMmpTransfer();
      virtual ~CMmpTransfer();
       
      void writeMMP( Tdt::EObject object, int pos, uint32_t value );
       
      void startTx()
      {
         m_data.reset();
         writeMMP( m_data.m_object, 0, m_data.data32() );
         m_timeoutTimer.start( m_shredTimeout );
      }
       
      void handleRx(Tdt::CMessage& msg);
      void handleTx(Tdt::CMessage& msg);
      
      void sendShred( );
      void sendAbort( );
      void sendAck( uint32_t pos=false );
      
      void handleMmpTransfer(CMmpTransferData& data);
      
      bool finished()
      {
         return( m_data.m_finished );
      }
};

#if 0

class CMmpTransferTx: public CMmpTransfer // , QObject
{
   //Q_OBJECT
   
   private:
       QTimer m_timeoutTimer;
       static constexpr const int m_shredTimeout=250;
       
   public:
       virtual ~CMmpTransferTx()
       {
          return;
       }
      void startTx()
      {
         m_td.reset();
         writeMMP( m_td.m_object, 0, m_td.data32() );
         m_timeoutTimer.start( m_shredTimeout );
      }
};


class CMmpTransferRx: public CMmpTransferData
{
   //Q_OBJECT
   
   public:
};

#endif

class CMmpNode: public QObject
{
   Q_OBJECT
   
   public:
      CMmpTransfer m_tx;
      CMmpTransfer m_rx;
      
  //signals:
      //void send(Tdt::CMessage& msg);
      
  public slots:
      void receive(Tdt::CMessage& msg);
      bool finishedRx()
      {
         return( m_rx.finished() );
      }
};


} // namespace Tdt


//---fin----------------------------------------------------------------------
#endif // ? ! TDT_MMP_HPP
