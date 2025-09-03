#ifndef LEPTO_CAN_MESSAGE_TDT_HPP
#define LEPTO_CAN_MESSAGE_TDT_HPP
/**---------------------------------------------------------------------------
 *
 * @file    message.hpp
 * @brief   The tdt message structures and functions
 *
 * @date      20241219
 * @author    Maximilian Seesslen <mes@seesslen.net>
 * @copyright SPDX-License-Identifier: Apache-2.0
 *
 *--------------------------------------------------------------------------*/


/*--- Includes -------------------------------------------------------------*/


#include <stdint.h>
#include <cassert>      // assert
#include <memory.h>     // memcpy

#if defined USE_LEPTO
   #include <lepto/log.h>     // ELogCategory
   enum class ELogCode: int;
#else
   #include <tdt/compat.hpp>
   enum class ELogCode: int32_t;
   enum class ELogCategory: int32_t;
#endif

#if defined USE_BIWAK
   #include <biwak/canMessage.hpp>
#else
   #include <tdt/canMessage.hpp>
#endif

#include <tdt/gen/objects.hpp>
#include <tdt/gen/units.hpp>
#include <tdt/gen/commands.hpp>
#include <tdt/gen/logs.hpp>


/*--- Declaration ----------------------------------------------------------*/


#define CAN_TDT_PROTOCOL_VERSION    4
   
#define TdtMatchesSubIndexedObject( objectIndex , objectEnum ) \
   (( objectIndex >= objectEnum ## Start ) && ( objectIndex <= objectEnum ## Max )  )

namespace Tdt
{

static constexpr unsigned int NODEID_BITMASK  =0x7F;
static constexpr uint32_t NMT_OBJECT_OFFSET=0x1000;
static constexpr uint32_t LOG_CODE_OFFSET=0x3000;
static constexpr unsigned int FUNCTIONCODE_BITSHIFT =7;
static constexpr unsigned int FUNCTIONCODE_BITMASK  =0x780;
static constexpr unsigned int FUNCTIONCODE_RSHIFTED_BITMASK  =0xF;


/**
 * @brief Function code of an TDT message
 * 
 * The NMT-Introduce is speccial because Node-ID and UID/MAC have to be within
 * a single message.
 * Other NMT messages from the master can be split (As long as there is only a
 * single NMT master.
 */
enum class EFunctionCode: uint32_t
{
   nmt                  = 0x0,
   nmtIntroduce         = 0x1,
   alert                = 0x2,
   reserved1            = 0x4,
   writeObjectBroadCast = 0x5,
   writeObject          = 0x6,
   readObject           = 0x7,
   sendObject           = 0x8,
   dataBlob             = 0x9,
   reSendObject         = 0xA,
   ackDataBlob          = 0xB,
   max                  = 0xF,   // Reserved for unspecified traffic; e.g. for
                                 // CANPong tests
};

static_assert( ( (int)EFunctionCode::max & FUNCTIONCODE_RSHIFTED_BITMASK )
                                                == (int)EFunctionCode::max
               , "Masks not plausible");

/**
 * @brief Fixed NMT objects
 * 
 * These are not generated at the moment. This may change.
 */
enum class ENmtObject: uint16_t
{
   nmtScan                    = 0x00 + NMT_OBJECT_OFFSET,
   nmtScanConfig              = 0x01 + NMT_OBJECT_OFFSET,
   nmtRecipientLowId          = 0x02 + NMT_OBJECT_OFFSET,
   nmtRecipientHighId         = 0x03 + NMT_OBJECT_OFFSET,
   nmtSetNodeId               = 0x04 + NMT_OBJECT_OFFSET,
   nmtBeckon                  = 0x05 + NMT_OBJECT_OFFSET,
   nmtReset                   = 0x06 + NMT_OBJECT_OFFSET,
   nmtCalm                    = 0x07 + NMT_OBJECT_OFFSET,
   nmtJumpBootLoader          = 0x08 + NMT_OBJECT_OFFSET,
   nmtJumpApplication         = 0x09 + NMT_OBJECT_OFFSET,
   //nmtEnableApplicationBoot   = 0x0A + NMT_OBJECT_OFFSET,
};


typedef uint32_t nodeId_t;


enum class EEvent: uint32_t;

#if 0
enum class EEvent: uint32_t
{
   null              = 0x0,
   #if 1

   // CAUTION: lower nibble must be 0 because log category is mangled in

   // routine control
   wannaSleepStart      = 0x10 + LOG_CODE_OFFSET,
   wannaSleepAll        = 0x11 + LOG_CODE_OFFSET,
   wannaSleepRoom0      = 0x12 + LOG_CODE_OFFSET,
   wannaSleepRoom1      = 0x13 + LOG_CODE_OFFSET,
   wannaSleepEnd        = 0x1F + LOG_CODE_OFFSET,
   noRelease_OBS        = 0x20 + LOG_CODE_OFFSET,
   shutdown             = 0x21 + LOG_CODE_OFFSET,
   // batteryLow           = 0x22 + LOG_CODE_OFFSET,
   // vbusLow              = 0x23 + LOG_CODE_OFFSET,
   // couldNotReadEeprom   = 0x24 + LOG_CODE_OFFSET,
   
   #endif
   noRelease            = 0x25 + LOG_CODE_OFFSET,
   test,                // Just for Catch2 tests
};
#endif

#if 0
enum class ELogCode2: uint32_t
{
   couldNotReadEeprom
};

#endif

enum class ESystemState: uint32_t
{
   invalid,
   bootloader,
   application,
   reverseBootloader,
};

inline constexpr Tdt::EEvent operator+ ( Tdt::EEvent e1, int i1 )
{
   return( (Tdt::EEvent)( (int)e1 + i1 ) );
}

inline constexpr ELogCode operator+ (ELogCategory c, Tdt::EEvent e)
{
   return( (ELogCode)( (int)c | ( (int)e  ) ) );
}

/*inline constexpr operator ELogBlended(ELogCategory c)
{
   return( (ELogBlended)( (int)c  ) );
}
*/
inline constexpr ELogCode operator| (ELogCategory c, Tdt::EEvent e)
{
   return( (ELogCode)( (int)c | ( (int)e  ) ) );
}

inline constexpr ELogCode operator+ (ELogCategory c, Tdt::ELog l)
{
   return( (ELogCode)( (int)c | ( (int)l  ) ) );
}

inline constexpr ELogCode operator| (ELogCategory c, Tdt::ELog l)
{
   return( (ELogCode)( (int)c | ( (int)l  ) ) );
}

inline constexpr Tdt::EEvent toEvent( ELogCode c )
{
   return( (Tdt::EEvent)((int)c & 0xFFFFFFF0 ) );
}

inline constexpr Tdt::ELog toLog( ELogCode c )
{
   return( (Tdt::ELog)((int)c  ) );
}

inline constexpr ELogCategory toCategory( ELogCode c )
{
   return( (ELogCategory)((int)c & 0xF ) );
}


#undef PHONY_PACKED
//#define PACKED __attribute ((packed))
#define PHONY_PACKED


/**
 * @brief The actual payload of an TDT message
 */
union SValue
{
      int32_t _int;
      uint32_t _uint;
      int32_t permill;
      int32_t zenti;
      struct SSoftwareVersion
      {
         struct {
            uint8_t dirty : 1;
            uint8_t bootloader : 1;
            uint8_t distance : 6;
         }drift;
         uint8_t patch;
         uint8_t minor;
         uint8_t major;
      } PHONY_PACKED softwareVersion;
      static_assert( sizeof(softwareVersion) == 4 );

      uint32_t softwareVersionCode;
      uint32_t timestamp;
      int32_t timestampdelta;
      float _float;
      bool _bool;
      struct
      {
         uint8_t day;
         uint8_t month;
         uint16_t year;
      } PHONY_PACKED date;
      static_assert( sizeof( date ) == 4 );

      struct STime
      {
         uint8_t sec;
         uint8_t min;
         uint8_t hour;
         uint8_t reserved;
      } PHONY_PACKED time;
      static_assert( sizeof( time ) == 4 );

      ECommand command;
      nodeId_t  nodeId;
      ELogCode logCode;
      EEvent event;
      ESystemState systemState;
};

static_assert( sizeof(SValue) == 4, "Size missmatch");


/**
 * @brief Somehow "overloaded" class of the SCanMessage. 
 * 
 * It brings the Tdt payload on top of the can message
 */
class SCanMessageTdt
{
   protected:
      union
      {
         SCanMessage m;
         struct
         {
            canId_t  m_id;
            int      m_len;
            // This can not be outsourced to an struct partially.
            union {  // 4 Byte
               struct {
                  union {
                     EObject object;      // 2 B
                     ENmtObject nmtObject;      // 2 B
                     //EMmpObject mmpObject;      // 2 B
                  };
                  EUnit unit;          // 1 Binclude/tdt/message.hpp
                  uint8_t reserved;    // 1 B
               } PHONY_PACKED;
               struct {
                  uint16_t mmpSource;
                  uint16_t mmpPos;
               } PHONY_PACKED;
            };
            SValue value __attribute ( ( aligned(4) ) );
            //STdtPayload m_tdtPayload;
         };
      };
   
   public:
      #if defined CONFIG_TDT_CONSTEXPR_DEFAULT_CONSTRUCTOR
      constexpr
      #endif
      SCanMessageTdt()
         :m()
      {};

      #if 0
      constexpr SCanMessageTdt( nodeId_t id )
         :m_id{ id }
      {
         //m.setId( id );
      };
      #endif
      constexpr SCanMessageTdt( const SCanMessage& msg)
          :m{msg}
          //,value{ ._uint=0 }
      {
         
      };

      constexpr SCanMessageTdt(nodeId_t id, EFunctionCode functionCode, EObject object
                         , Tdt::EUnit unit, const SValue value = {._uint=0} )
         :m_id( id | ( (unsigned int)functionCode << FUNCTIONCODE_BITSHIFT ) )
         ,m_len( 8 ) // sizeof(m.m_data)
         ,object( object )
         ,unit( unit )
         ,reserved( 0 )
         ,value( value )

      {

      }
      constexpr SCanMessageTdt(nodeId_t id, EFunctionCode functionCode
                           , uint16_t _mmpSource, uint16_t _mmpPos, uint32_t value)
         :m_id( id | ( (unsigned int)functionCode << FUNCTIONCODE_BITSHIFT ) )
         ,m_len( 8 ) // sizeof(m.m_data)
         ,mmpSource( _mmpSource )
         ,mmpPos( _mmpPos )
         ,value( {._uint = value } )
      {

      }

      constexpr int getLen() const
      {
         return( m.getLen() );
      }
      constexpr canId_t getId() const
      {
         return( m.getId() );
      }
};


/**
 * @brief The final class of an TDT message
 */
class CMessage : public SCanMessageTdt
{
      friend class CMessageRef;

   private:

      static_assert ( sizeof(SCanMessageTdt) == sizeof(SCanMessage), "Size missmatch" );

   public:
      #if defined CONFIG_TDT_CONSTEXPR_DEFAULT_CONSTRUCTOR
      constexpr
      #endif
      CMessage()
      {
      }
      constexpr CMessage(nodeId_t id, EFunctionCode functionCode, EObject object
                         , Tdt::EUnit unit, const SValue value = {._uint=0} )
         :SCanMessageTdt( id,
            functionCode, object, unit, value)
      {
      }
      constexpr CMessage(nodeId_t id, EFunctionCode functionCode
               , uint16_t mmpSource, uint16_t mmpPos, uint32_t value)
         :SCanMessageTdt(id, functionCode, mmpSource, mmpPos, value)
      {
      }
      constexpr CMessage( const SCanMessage& msg)
         :SCanMessageTdt(msg)
      {
      }
      // Needed to create valid copy.
      CMessage( const CMessage &msg )
          :SCanMessageTdt(msg)
      {
      };
      // Needed in unit tests
      CMessage& operator=(const CMessage& msg)
      {
         m.setId(msg.getId());
         m.setData(msg.getLen(), msg.getData());
         return(*this);
      }
      EUnit getTdtUnit() const
      {
         return( unit );
      }
      EObject getTdtObject() const
      {
         return( object );
      }
      /*
      EMmpObject getTdtMmpObject() const
      {
         return(m_tdtMessage.mmpObject);
      }
      */
      int getTdtSubId() const
      {
         return( (int)object & 7 );
      }
      int32_t getTdtValueInt() const
      {
         return( value._int );
      }
      uint32_t getTdtValueUInt() const
      {
         return( value._uint );
      }
      float getTdtValueFloat() const
      {
         return( value._float );
      }
      CMessage& operator << (float fvalue)
      {
         value._float=fvalue;
         return(*this);
      }
      const SValue *getTdtValue() const
      {
         return( &value );
      }
      SValue *getTdtValue()
      {
         return( &value );
      }
      void setObject( EObject _object)
      {
         object=_object;
      }

      constexpr unsigned int getNodeId() const
      {
         return( m.getId() & NODEID_BITMASK );
      }
      void setNodeId( unsigned int id )
      {
         return( m.setId( id ) );
      }
      constexpr EFunctionCode getFunctionCode() const
      {
         return( (EFunctionCode)
                 ( ( m.getId() >> FUNCTIONCODE_BITSHIFT )
                        & FUNCTIONCODE_RSHIFTED_BITMASK )
                 );
      }
      void setFunctionCode(EFunctionCode functionCode)
      {
         m.setId( ( m.getId() & NODEID_BITMASK )
                        | ( (int)functionCode << FUNCTIONCODE_BITSHIFT ) );
         return;
      }
      void setUnit(EUnit _unit)
      {
         unit=_unit;
      }
      void setObject(EObject _object, EUnit _unit, SValue _value)
      {
         object=_object;
         unit=_unit;
         value=_value;
      }
      void setValue(SValue _value)
      {
         value=_value;
      }
      uint16_t getMmpPos() const
      {
         return( mmpPos );
      }
      bool isMultiCast() const
      {
         return( getFunctionCode() == EFunctionCode::writeObjectBroadCast );
      }
      operator const SCanMessage& () const
      {
         return( m );
      }
      // needed in cordyceps
      SCanMessage &canMessage()
      {
         //return *(dynamic_cast<SCanMessage*>(this));
         return(m);
      }
      const uint8_t* getData() const
      {
         return( m.getData() );
      }
};


EObject operator+(const EObject &a, int value);

// Needed e.g. for storing relativ object-offsets
inline constexpr int operator-(const EObject &a, EObject b)
{
   return( (int)a - (int)b );
}


};  // namespace Tdt


/*--- Fin ------------------------------------------------------------------*/
#endif // ? ! LEPTO_CAN_MESSAGE_TDT_HPP
