#ifndef LEPTO_CAN_MESSAGE_TDT_HPP
#define LEPTO_CAN_MESSAGE_TDT_HPP
//-----------------------------------------------------------------------------
//
//
//
//-----------------------------------------------------------------------------


#include <stdint.h>
#include <cassert>      // assert
#include <memory.h>     // memcpy
#include <cassert>


#if defined USE_LEPTO
   //#include <lepto/can_message.h>
   #include <lepto/log.h>     // ELogCategory
   enum class ELogBlended: int;
#else
   #include <tdt/compat.hpp>
   enum class ELogBlended: int32_t;
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

#define CAN_TDT_PROTOCOL_VERSION    4

namespace Tdt
{

static constexpr unsigned int NODEID_BITMASK  =0x7F;
static constexpr uint32_t NMT_OBJECT_OFFSET=0x1000;
static constexpr uint32_t LOG_CODE_OFFSET=0x3000;
static constexpr unsigned int FUNCTIONCODE_BITSHIFT =7;
static constexpr unsigned int FUNCTIONCODE_BITMASK  =0x780;
static constexpr unsigned int FUNCTIONCODE_RSHIFTED_BITMASK  =0xF;


/*
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
   nmtEnableApplicationBoot   = 0x0A + NMT_OBJECT_OFFSET,
};


typedef uint32_t nodeId_t;


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
   batteryLow           = 0x22 + LOG_CODE_OFFSET,
   vbusLow              = 0x23 + LOG_CODE_OFFSET,
   
   #endif
   couldNotReadEeprom,
   noRelease,
};

enum class ELogCode2: uint32_t
{
   couldNotReadEeprom
};


enum class ESystemState: uint32_t
{
   invalid,
   bootLoader,
   application
};


inline constexpr Tdt::EEvent operator+ ( Tdt::EEvent e1, int i1 )
{
   return( (Tdt::EEvent)( (int)e1 + i1 ) );
}

inline constexpr ELogBlended operator+ (ELogCategory c, Tdt::EEvent e)
{
   return( (ELogBlended)( (int)c | ( (int)e << 4 ) ) );
}

inline constexpr ELogBlended operator| (ELogCategory c, Tdt::EEvent e)
{
   return( (ELogBlended)( (int)c | ( (int)e << 4 ) ) );
}

inline constexpr Tdt::EEvent toEvent( ELogBlended c )
{
   return( (Tdt::EEvent)((int)c >> 4 ) );
}

//#define PACKED __attribute ((packed))
#define PACKED

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
      } PACKED softwareVersion;
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
      } PACKED date;
      static_assert( sizeof( date ) == 4 );

      struct STime
      {
         uint8_t sec;
         uint8_t min;
         uint8_t hour;
         uint8_t reserved;
      } PACKED time;
      static_assert( sizeof( time ) == 4 );

      ECommand command;
      nodeId_t  nodeId;
      ELogBlended logBlended;
      EEvent event;
      ESystemState systemState;
};

static_assert( sizeof(SValue) == 4, "Size missmatch");

struct SMessage
{
   union {
      struct {
         union {
            EObject object;      // 2 B
            ENmtObject nmtObject;      // 2 B
            //EMmpObject mmpObject;      // 2 B
         };
         EUnit unit;          // 1 Binclude/tdt/message.hpp
         uint8_t reserved;    // 1 B
      } PACKED;
      struct {
         uint16_t mmpSource;
         uint16_t mmpPos;
      } PACKED;
   };
   SValue value __attribute ( ( aligned(4) ) );
   static_assert( sizeof(value) == 4, "Message size not plausible" );
} PACKED;

static_assert( sizeof( SMessage ) == 8 );

#define constexpr_nobug constexpr


class SCanMessageTdt
{
   protected: 
      union{
         SCanMessage m;
         struct
         {
            uint32_t reserved1;
            uint32_t reserved2;
            SMessage m_tdtMessage;
         };
      };
   
   public:
      constexpr SCanMessageTdt()
         :m{}
      {};
      constexpr SCanMessageTdt( nodeId_t id )
         :m{ id }
      {
         //m.setId( id );
      };
      constexpr SCanMessageTdt( const SCanMessage& msg)
          :m{msg}
      {
         
      }
      int getLen()
      {
         return( m.getLen() );
      }
      constexpr canId_t getId() const
      {
         return( m.getId() );
      }
};

#if 0
struct
{
   union{
      EObject object;
      EMmpObject mmpObject;
   };
}EAnyObject;
#endif

class CMessage : public SCanMessageTdt
{
      friend class CMessageRef;

   private:

      static_assert ( sizeof(SMessage) == 4 + 4, "Size missmatch" );
       
   public:


      constexpr CMessage()
      {
      }
      
      constexpr CMessage(nodeId_t id, EFunctionCode functionCode, EObject object, Tdt::EUnit unit)
         :SCanMessageTdt( id | ( (unsigned int)functionCode << FUNCTIONCODE_BITSHIFT ))
      {
         assert( id <= NODEID_BITMASK );
         m_tdtMessage.object=object;
         m_tdtMessage.unit=unit;
         m.setLen( sizeof(SMessage) );
      }

      constexpr CMessage(nodeId_t id, EFunctionCode functionCode, EObject object
                     , EUnit unit, const SValue value)
         :CMessage(id, functionCode, object, unit )
      {
         m_tdtMessage.value=value;
      }
      constexpr CMessage(nodeId_t id, EFunctionCode functionCode, EObject object
               , uint16_t mmpPos, uint32_t value)
          :SCanMessageTdt(id | ( (unsigned int)functionCode << FUNCTIONCODE_BITSHIFT ))
      {
         m_tdtMessage.object=object;
         m_tdtMessage.mmpPos=mmpPos;
         m_tdtMessage.value._uint=value;
         m.setLen( sizeof(SMessage) );
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
   #if 0
      // Needed in unit tests
      CMessage& operator=(const CMessage& msg)
      {
         setId(msg.getId());
         setData(msg.getLen(), msg.getData());
         return(*this);
      }
   #endif
      EUnit getTdtUnit() const
      {
         return(m_tdtMessage.unit);
      }
      EObject getTdtObject() const
      {
         return(m_tdtMessage.object);
      }
      /*
      EMmpObject getTdtMmpObject() const
      {
         return(m_tdtMessage.mmpObject);
      }
      */
      int getTdtSubId() const
      {
         return( (int)m_tdtMessage.object & 7 );
      }
      int32_t getTdtValueInt() const
      {
         return(m_tdtMessage.value._int);
      }
      uint32_t getTdtValueUInt() const
      {
         return(m_tdtMessage.value._uint);
      }
      float getTdtValueFloat() const
      {
         return(m_tdtMessage.value._float);
      }
      CMessage& operator << (float value)
      {
         m_tdtMessage.value._float=value;
         return(*this);
      }
      const SValue *getTdtValue() const
      {
         return( &m_tdtMessage.value );
      }
      SValue *getTdtValue()
      {
         return( &m_tdtMessage.value );
      }
      void setObject( EObject object)
      {
         m_tdtMessage.object=object;
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
      void setUnit(EUnit unit)
      {
         m_tdtMessage.unit=unit;
      }
      void setObject(EObject object, EUnit unit, SValue value)
      {
         m_tdtMessage.object=object;
         m_tdtMessage.unit=unit;
         m_tdtMessage.value=value;
      }
      void setValue(SValue value)
      {
         m_tdtMessage.value=value;
      }
      uint16_t getMmpPos() const
      {
         return( m_tdtMessage.mmpPos );
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


//---fin-----------------------------------------------------------------------
#endif // ? ! LEPTO_CAN_MESSAGE_TDT_HPP
