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
 * @author    Maximilian Seesslen <src@seesslen.net>
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
   #if ! defined USE_BIWAK
      #error TDT-MMP: On STM32 devices i need libbiwak. Is the order of submodules in CMakeLists.txt correct?
   #endif
   #include <biwak/sysTimer.hpp>
   #include <biwak/softtimer.h>
#else
   #include <QObject>
   #include <QTimer>
#endif


/*--- Declaration ----------------------------------------------------------*/


#if USE_LEPTO
   #if 0
      #if defined( STM32 ) && IS_ENABLED( CONFIG_TDT_MMP_CALLBACKS )
         void cbSendTdtMessage( const Tdt::CMessage& );
         Tdt::nodeId_t cbGetNodeId();
         int cbHandleMmpTransfer( const Tdt::CMmpTransferData& );
      #endif
   #endif
#else
   #define IS_ENABLED( A ) A
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

enum class EReturnCode
{
   invalid           = 0,
   ok                = 1,
   transferOngoing   = -100,
   error             = -101,
   counterPartDead   = -102,
   unknownCommand    = -103,
   wrongCRC          = -104,
   notImplemented    = -105,
   unhandled         = -106,
   wrongFlashArea    = -107,
   errorFlashing     = -108,
   invalidInit       = -200,
   
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
static_assert( sizeof(SMmpHeader) % 4 == 0 );

enum class ENodeState
{
   idle=0,
   receiving=1,
   transmitting=2,
   receivingTansmitting=3,
};


// Unit tests want these operators to verify state
#if ! defined STM32

   #if 1

   static Tdt::ENodeState operator |( Tdt::ENodeState a, Tdt::ENodeState b )
   {
      return( (Tdt::ENodeState)( (int)a | (int)b ) );
   }

   #endif

   #if 0

   static Tdt::ENodeState operator &( Tdt::ENodeState a, Tdt::ENodeState b )
   {
      return( (Tdt::ENodeState)( (int)a & (int)b ) );
   }

   #endif

#endif


} // namespace Tdt


/*--- Fin ------------------------------------------------------------------*/
#endif // ? ! TDT_MMP_HPP
