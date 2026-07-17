/**---------------------------------------------------------------------------
 *
 * @file    mmpNode.cpp
 * @brief   Multi-message-package support for TDT protocol
 *
 * Transfer BLOBs from one device to another.
 * 
 * @date      20241003
 * @author    Maximilian Seesslen <src@seesslen.net>
 * @copyright SPDX-License-Identifier: Apache-2.0
 *
 *--------------------------------------------------------------------------*/


/*--- Includes -------------------------------------------------------------*/


#include <tdt/mmpTransfer.hpp>

#if ( ! defined STM32 ) && defined ( USE_LEPTO )
   #include <lepto/print.h>      // hexDump
#endif


/*--- Implementation -------------------------------------------------------*/


namespace Tdt
{

void CMmpTransfer::updateCrc()
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
                                          , m_rawData, m_header.dataLength );
         m_header.crc32Data = crc32Finalize( m_header.crc32Data );
      #else
         m_header.crc32Data = 0;
      #endif
   }
};


}; // namespace Tdt


/*--- Fin ------------------------------------------------------------------*/
