#ifndef LEPTO_CAN_MESSAGE_TDT_H_
#define LEPTO_CAN_MESSAGE_TDT_H_
//-----------------------------------------------------------------------------
//
//
//
//-----------------------------------------------------------------------------


#include <stdint.h>
#include <cassert>      // assert
#include <memory.h>     // memcpy

#if defined USE_LEPTO
   // #include <lepto/log.h>
   // #include <lepto/can_message.h>
#else
   #include <tdt/compat.hpp>
   enum class ELogCode: int32_t;
   enum class ELogCategory: int32_t;
#endif

#include <tdt/gen/objects.hpp>
#include <tdt/gen/units.hpp>

#define CAN_TDT_PROTOCOL_VERSION    3

namespace Tdt
{

static constexpr int OBJECT_OFFSET=0x400;
static constexpr int NMT_OBJECT_OFFSET=0x1000;
static constexpr int MMP_OBJECT_OFFSET=0x6000;
static constexpr int UNIT_OFFSET=0x10;
static constexpr int TYPE_OFFSET=0x20;
static constexpr int COMMAND_OFFSET=0x600;
static constexpr int EVENT_OFFSET=0x700;

static constexpr int NO_SUBID_OBJECT_MASK = 0xFFF8;
static constexpr int NODEID_BITMASK  =0x7F;

static constexpr int FUNCTIONCODE_BITSHIFT =7;
// https://www.microcontrol.net/wp-content/uploads/2021/10/canopen_intro.pdf
// https://www.waycon.de/fileadmin/seilzugsensoren/CANopen-Handbuch.pdf
// CANOpen has 4 Bits function code + 7 bits Node ID
static constexpr int FUNCTIONCODE_BITMASK  =0x780;
static constexpr int FUNCTIONCODE_RSHIFTED_BITMASK  =0xF;
static constexpr int SUBID_MASK = 0x7;
// 0x00 .. 0xF can be regular SUB-Ids
static constexpr int OBJECT_NO_SUBID_BITMASK=0xFFF8;
static constexpr int OBJECT_SUBID_BITMASK=0x7;

static constexpr int BORADCAST_NODE_ID_LP = 120;
static constexpr int BORADCAST_NODE_ID_HP = 2;

static constexpr uint32_t MASK_SUBINDEX        = 0xF;
static constexpr uint32_t SUBINDEX_COUNT       = 0x10;
static constexpr uint32_t MASK_SUBINDEX_OBJECT = ~MASK_SUBINDEX;
static constexpr uint32_t LAST_SUBINDEX        = 0xF;

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
   //null                       = 0x00,
   //error                      = 0x00,
   
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
   //nmtStatus                   = 0x10 + NMT_OBJECT_OFFSET,
};


#if 0
static_assert ( ( (int)EObject::plantSensor & NO_SUBID_OBJECT_MASK )
                                       == (int)EObject::plantSensor,"" );
#endif

#if 0
static_assert ( ( (int)EObject::temperature & NO_SUBID_OBJECT_MASK )
                                       == (int)EObject::temperature,"" );
#endif

constexpr bool matchesSubIndexedObject( const Tdt::EObject object, const Tdt::EObject base )
{
   return( ( (uint32_t)object & MASK_SUBINDEX_OBJECT ) == (uint32_t)base );
}


typedef int32_t nodeId_t;


enum class ECommand: uint32_t
{
   null                    = 0x0,
   replay                  = 0x1 + COMMAND_OFFSET,
   destroy                 = 0x2 + COMMAND_OFFSET,
   burnIn                  = 0x3 + COMMAND_OFFSET,
   forceMeasure            = 0x4 + COMMAND_OFFSET,
   ident                   = 0x5 + COMMAND_OFFSET,
   burnInShort             = 0x6 + COMMAND_OFFSET,
   burnInLong              = 0x7 + COMMAND_OFFSET,
   bootToRecovery          = 0x8 + COMMAND_OFFSET,
   bootToProductive        = 0x9 + COMMAND_OFFSET,
   keepAwakeStart          = 0x10 + COMMAND_OFFSET,
   keepAwakeAll            = 0x11 + COMMAND_OFFSET,
   keepAwakeRoom0          = 0x12 + COMMAND_OFFSET,
   keepAwakeEnd            = 0x1F + COMMAND_OFFSET,
   measureLoop             = 0x20 + COMMAND_OFFSET,
   enableBootAplpication   = 0x21 + COMMAND_OFFSET,
   unknownCommand    = 0x1000 + COMMAND_OFFSET,
};

enum class EEvent: uint32_t
{
   null              = 0x0,

   // CAUTION: lower nibble must be 0 because log category is mangled in

   // routine control
   wannaSleepStart      = 0x10 + EVENT_OFFSET,
   wannaSleepAll        = 0x11 + EVENT_OFFSET,
   wannaSleepRoom0      = 0x12 + EVENT_OFFSET,
   wannaSleepRoom1      = 0x13 + EVENT_OFFSET,
   wannaSleepEnd        = 0x1F + EVENT_OFFSET,
   noRelease_OBS        = 0x20 + EVENT_OFFSET,
   shutdown             = 0x21 + EVENT_OFFSET,
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

inline constexpr ELogCode operator+ (ELogCategory c, Tdt::EEvent e)
{
   return( (ELogCode)( (int)c | ( (int)e << 4 ) ) );
}

inline constexpr ELogCode operator| (ELogCategory c, Tdt::EEvent e)
{
   return( (ELogCode)( (int)c | ( (int)e << 4 ) ) );
}

inline constexpr Tdt::EEvent toEvent( ELogCode c )
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
      ELogCode logCode;
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
         EUnit unit;          // 1 B
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


struct SMessageNmtIntro
{
   uint32_t uid_low;
   uint32_t uid_high;
} PACKED;

static_assert( sizeof( SMessageNmtIntro ) == 8, "Message (NMT Intro) size not plausible" );

#define constexpr_nobug constexpr

class CCanMessage
{
      nodeId_t m_id;
      size_t m_len;
  protected:
      union{
         uint8_t m_data[8];
         SMessage m_tdtMessage;
      };
      
   public:
      constexpr CCanMessage()
           :m_id{0}
           ,m_len{0}
           ,m_data{0}
       {}
      constexpr CCanMessage(nodeId_t id)
         :m_id{id}
         ,m_len{0}
         ,m_data{0}
      {}
      constexpr CCanMessage(nodeId_t id, size_t len, const unsigned char* data)
          :m_id{id}
          ,m_len{len}
          ,m_data{ data[0], data[1], data[2], data[3], data[4], data[5], data[6], data[7] }
      {
      }
      void setId( nodeId_t id )
      {
         m_id = id;
      }
      constexpr nodeId_t getId() const
      {
         return( m_id );
      }
      constexpr void setLen(size_t len)
      {
         m_len = (len>8) ? 8 : len;
      }
      size_t getLen() const
      {
         return( m_len );
      }
      void setData( size_t len, const void* data )
      {
         m_len = (len>8) ? 8 : len;
         memcpy(m_data, data, m_len);
      }
      const uint8_t* getData() const
      {
         return( m_data );
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

class CMessage : public CCanMessage
{
      friend class CMessageRef;

private:
      static constexpr int EID_MULTIPLIER=0x100;
      static constexpr int EID_SUBID_MASK=0xFF;

      static_assert ( sizeof(SMessage) == 4 + 4, "Size missmatch" );
      //SMessage &m_tdtMessage=*static_cast<SMessage *>(getData());
      //SMessage *m_tdtMessage=(SMessage *)(getData());

   public:

      constexpr_nobug CMessage()
         :CCanMessage{ (nodeId_t)0ul }
      {
      }
#if 1
      constexpr_nobug CMessage(nodeId_t id, EFunctionCode functionCode, EObject object, Tdt::EUnit unit)
         :CCanMessage( id | ( (unsigned int)functionCode << FUNCTIONCODE_BITSHIFT ))
      {
         assert( id <= NODEID_BITMASK );
         m_tdtMessage.object=object;
         m_tdtMessage.unit=unit;
         setLen( sizeof(SMessage) );
      }
#endif
      constexpr_nobug CMessage(nodeId_t id, EFunctionCode functionCode, EObject object
                     , EUnit unit, const SValue value)
         :CMessage(id, functionCode, object, unit )
      {
         m_tdtMessage.value=value;
      }
      constexpr_nobug CMessage(nodeId_t id, EFunctionCode functionCode, EObject object
               , uint16_t mmpPos, uint32_t value)
          :CCanMessage(id | ( (unsigned int)functionCode << FUNCTIONCODE_BITSHIFT ))
      {
         m_tdtMessage.object=object;
         m_tdtMessage.mmpPos=mmpPos;
         m_tdtMessage.value._uint=value;
         setLen( sizeof(SMessage) );
      }
      constexpr_nobug CMessage( const CCanMessage& msg)
         :CCanMessage(msg)
      {
      }
      // Needed to create valid copy.
      CMessage( const CMessage &msg )
          :CCanMessage(msg)
      {
         
      };
      
      // Needed in unit tests
      CMessage& operator=(const CMessage& msg)
      {
         setId(msg.getId());
         setData(msg.getLen(), msg.getData());
         return(*this);
      }
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
         return( getId() & NODEID_BITMASK );
      }
      void setNodeId( unsigned int id )
      {
         return( setId( id ) );
      }
      EFunctionCode getFunctionCode() const
      {
         return( (EFunctionCode)
                 ( ( getId() >> FUNCTIONCODE_BITSHIFT )
                        & FUNCTIONCODE_RSHIFTED_BITMASK )
                 );
      }
      void setFunctionCode(EFunctionCode functionCode)
      {
         setId( ( getId() & NODEID_BITMASK )
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
};


EObject operator+(const EObject &a, int value);
int operator-(const EObject &a, EObject b);

};  // namespace Tdt


//---fin-----------------------------------------------------------------------
#endif // ? ! LEPTO_CAN_MESSAGE_TDT_H_
