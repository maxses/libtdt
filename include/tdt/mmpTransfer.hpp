#ifndef TDT_MMP_TRANSFER_HPP
#define TDT_MMP_TRANSFER_HPP
/**---------------------------------------------------------------------------
 *
 * @file    mmpTransfer.hpp
 * @brief   Multi-message-package support for TDT protocol
 *
 * Transfer BLOBs from one device to another.
 * 
 * Handling the shreds directly (writing them to flash) are making CRC
 * handling impossible. Retransmitting an transfer does not work any 
 * more because the flash would be needed to erased again.
 * 
 * @date      20241003
 * @author    Maximilian Seesslen <src@seesslen.net>
 * @copyright SPDX-License-Identifier: Apache-2.0
 *
 *--------------------------------------------------------------------------*/


/*--- Includes -------------------------------------------------------------*/


#include <tdt/mmp.hpp>


/*--- Declaration ----------------------------------------------------------*/


namespace Tdt
{

class CMmpTransfer
{
   private:
      SMmpHeader m_header;
      char* m_rawData;
      int m_maxReceiveSize;
      int m_retry=0;
      nodeId_t m_counterNodeId;
      EReturnCode m_returnCode = EReturnCode::invalidInit;

   public:
      enum class EDirection
      {
         invalid,
         in,
         out,
      };

   private:
      #if IS_ENABLED( CONFIG_TDT_PEDANTIC )
         EDirection m_direction=EDirection::invalid;
         ENodeState m_state=ENodeState::idle;
      #endif
      int m_pos;

  public:
      //Tdt::EObject m_object=Tdt::EObject::nodeFirmwareDate;

   public:
      CMmpTransfer( EDirection direction )
      {
         m_header.dataLength=0;
         m_header.mmpCommand=EMmpCommand::null;
         m_maxReceiveSize=0x0;
         m_counterNodeId=0;
         m_rawData=nullptr;
         #if IS_ENABLED( CONFIG_TDT_PEDANTIC )
            m_direction=direction;
         #endif
         reset();

         if( direction == EDirection::in )
         {
            allocRx();
         }
      };

      /**
       * @brief Allocate memory for receiving a transfer
       */
      void allocRx()
      {
         #if ! defined STM32
            m_maxReceiveSize=0x10000;
         #else
            m_maxReceiveSize=0x200;
         #endif
         m_rawData=(char*)malloc( m_maxReceiveSize );
         reset();
      };

      /**
       * @brief Overwrite memory area for receiving transfer
       *
       * This is used in unit tests to verify that transfers are correct.
       */
      void wipe()
      {
         if( m_rawData )
         {
            memset( m_rawData, 0x77, m_maxReceiveSize );
         }
         m_returnCode = EReturnCode::invalidInit;
         m_counterNodeId=0;
      }

      /**
       * @brief Updates the CRC in the header
       */
      void updateCrc();
      int pos() const
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
         if( m_pos * 4 < (int)sizeof(SMmpHeader) )
            return( ( (uint32_t*)&m_header ) [ m_pos ] );
         if(!m_rawData)
         {
            qFatal("NDTS");
         }
         return( ( (uint32_t*)m_rawData )[ m_pos - ( sizeof(SMmpHeader) / 4 ) ] );
      };

      const char* data() const
      {
         return(m_rawData);
      }

      /**
       * @brief Go to the next transmition/receive position
       */
      void inc()
      {
         m_retry=0;
         m_pos++;
      }

      void reset()
      {
         // qDebug("Reset data");
         m_retry=0;
         m_pos=0;
         
         // Must not reset m_counterNodeId; the tx channel will still use it
         
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
             .crc32Data=0,
             .mmpCommand=command,
             .flashAddress=0,
             .reserved{0},
             //.mmpFunctionCode=0,
         };
         m_rawData=(char*)data;
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
      void setCounterNodeId( int nodeId )
      {
         m_counterNodeId=nodeId;
      }
      nodeId_t getCounterNodeId( )
      {
         return( m_counterNodeId );
      }
      bool isLast( int _pos ) const
      {
         return( isFinished( _pos + 1 ) );
      }

      bool isLast( ) const
      {
         return( isLast( pos() ) );
      }

      bool isFinished( int _pos ) const
      {
         return( ( _pos * 4 >= (int)sizeof( Tdt::SMmpHeader ) )
             && ( _pos * 4
                     >= (int)sizeof( Tdt::SMmpHeader ) + header().dataLength )
             );
      }
      bool isFinished( ) const
      {
         return( isFinished( pos() ));
      }

      #if ! defined STM32
      void dump() const
      {
         printf("   # %s Pos:%d\n",
            #if IS_ENABLED( CONFIG_TDT_PEDANTIC )
                m_direction==EDirection::out ? "TX" : "RX"
            #else
                "--"
            #endif
            , m_pos);
      }
      #endif
      
      #if IS_ENABLED( CONFIG_TDT_PEDANTIC )
      ENodeState state()
      {
         return( m_state );
      }
      
      void setState( ENodeState state )
      {
         m_state = state;
      }
      #endif

      void setReturnCode( EReturnCode returnCode )
      {
         m_returnCode = returnCode;
      }
      EReturnCode returnCode( ) const
      {
         return( m_returnCode );
      }
};


} // namespace Tdt


/*--- Fin ------------------------------------------------------------------*/
#endif // ? ! TDT_MMP_TRANSFER_HPP
