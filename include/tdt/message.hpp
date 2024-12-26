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

enum class EObjectObsolete: uint16_t
{
   none                       = 0x00 + OBJECT_OFFSET,
   date                       = 0x05 + OBJECT_OFFSET,
   time                       = 0x06 + OBJECT_OFFSET,
   dummy                      = 0x07 + OBJECT_OFFSET,
   
   firmwareVersion            = 0x10 + OBJECT_OFFSET,
   firmwareDate               = 0x11 + OBJECT_OFFSET,
   hardwareRevision           = 0x12 + OBJECT_OFFSET,
   hardwareDate               = 0x13 + OBJECT_OFFSET,
   humidity                   = 0x14 + OBJECT_OFFSET,
   voc                        = 0x15 + OBJECT_OFFSET,
   iaq                        = 0x16 + OBJECT_OFFSET,
   voc2                       = 0x17 + OBJECT_OFFSET,

   temperature                = 0x18 + OBJECT_OFFSET,
   temperature0               = 0x18 + OBJECT_OFFSET,
   temperature1               = 0x19 + OBJECT_OFFSET,
   temperature2               = 0x1A + OBJECT_OFFSET,
   temperature3               = 0x1B + OBJECT_OFFSET,
   temperature4               = 0x1C + OBJECT_OFFSET,

   batteryLevel               = 0x20 + OBJECT_OFFSET,
   pressure                   = 0x21 + OBJECT_OFFSET,
   nodeCommand                = 0x30 + OBJECT_OFFSET,
   timeStamp                  = 0x31 + OBJECT_OFFSET,
   current                    = 0x32 + OBJECT_OFFSET,
   currentAvg                 = 0x33 + OBJECT_OFFSET,
   SupplyVoltage              = 0x34 + OBJECT_OFFSET,
   debug                      = 0x35 + OBJECT_OFFSET,
   nodeId                     = 0x36 + OBJECT_OFFSET,
   articleId                  = 0x37 + OBJECT_OFFSET,
   serialNumber               = 0x38 + OBJECT_OFFSET,
   uidLow                     = 0x39 + OBJECT_OFFSET,
   uidHigh                    = 0x3A + OBJECT_OFFSET,
   chipId                     = 0x3B + OBJECT_OFFSET,
   flashSize                  = 0x3C + OBJECT_OFFSET,
   
   forcedSending              = 0x40 + OBJECT_OFFSET,
   
   foodTemperature            = 0x50 + OBJECT_OFFSET,
   
   // Candis: whose temperatures should be shown in display
   displayNode                = 0x60 + OBJECT_OFFSET,
   displayNode0               = 0x60 + OBJECT_OFFSET,
   displayNode1               = 0x61 + OBJECT_OFFSET,
   displayNode2               = 0x62 + OBJECT_OFFSET,
   displayNode3               = 0x63 + OBJECT_OFFSET,
   displayNode0ObjectOffset   = 0x64 + OBJECT_OFFSET,
   displayNode1ObjectOffset   = 0x65 + OBJECT_OFFSET,
   displayNode2ObjectOffset   = 0x66 + OBJECT_OFFSET,
   displayNode3ObjectOffset   = 0x67 + OBJECT_OFFSET,
   
   switchObjectOffset         = 0x70 + OBJECT_OFFSET,
   switchObjectOffset0        = 0x70 + OBJECT_OFFSET,
   switchObjectOffset1        = 0x71 + OBJECT_OFFSET,
   switchObjectOffset2        = 0x72 + OBJECT_OFFSET,
   switchObjectOffset3        = 0x73 + OBJECT_OFFSET,
   
   deviceStatus               = 0x80 + OBJECT_OFFSET,
   errorCode                  = 0x81 + OBJECT_OFFSET,
   eventCode                  = 0x82 + OBJECT_OFFSET,
   flags                      = 0x83 + OBJECT_OFFSET,
   bootCounter                = 0x84 + OBJECT_OFFSET,
   operatingTime              = 0x85 + OBJECT_OFFSET,
   shutdownTime               = 0x86 + OBJECT_OFFSET,
   releaseState               = 0x87 + OBJECT_OFFSET,
   fadeTime                   = 0x88 + OBJECT_OFFSET,
   eventCodeRepeated          = 0x89 + OBJECT_OFFSET,
   logCode                    = 0x8A + OBJECT_OFFSET,
   lastError                  = 0x8B + OBJECT_OFFSET,
   systemState                = 0x8C + OBJECT_OFFSET,

   startSwitches              = 0x90 + OBJECT_OFFSET,    // None
   startAmbientSwitches       = 0x90 + OBJECT_OFFSET,    // None
   ambientLightAll            = 0x91 + OBJECT_OFFSET,
   ambientLightLivingRoom     = 0x92 + OBJECT_OFFSET,
   ambientLightSleepingRoom   = 0x93 + OBJECT_OFFSET,
   ambientLightKitchen        = 0x94 + OBJECT_OFFSET,
   ambientLightOffice         = 0x95 + OBJECT_OFFSET,
   ambientLightBath1          = 0x96 + OBJECT_OFFSET,
   ambientLightBath2          = 0x97 + OBJECT_OFFSET,
   plantLight                 = 0xA0 + OBJECT_OFFSET,
   multimediaSwitch           = 0xA1 + OBJECT_OFFSET,
   mainSwitch                 = 0xA2 + OBJECT_OFFSET,
   endSwitches                = 0xBF + OBJECT_OFFSET,
   
   room                       = 0xC0 + OBJECT_OFFSET,
   
   dateStamp                  = 0xD5 + OBJECT_OFFSET, // days since 1.1.1970 UTC
   clockStamp                 = 0xD6 + OBJECT_OFFSET, // seconds since 0:0:0 UTC
   
   rtcDrift                   = 0x5000 + OBJECT_OFFSET,
   timeStampDrift             = 0x5001 + OBJECT_OFFSET,
};


enum EMmpObject_obsolete: uint32_t
{
   acknowledgeShred           = 0x01 + MMP_OBJECT_OFFSET,
   acknowledgeTransfer        = 0x02 + MMP_OBJECT_OFFSET,
   reset                      = 0x06 + MMP_OBJECT_OFFSET,
   jumpBootLoader          = 0x08 + MMP_OBJECT_OFFSET,
   jumpApplication         = 0x09 + MMP_OBJECT_OFFSET,
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

/*
static_assert ( matchesSubIndexedObject(
                  Tdt::EObject::ambientLight, Tdt::EObject::ambientLight) == true);
*/

typedef int32_t nodeId_t;

enum class EUnitObsolete: uint8_t
{
   null                 = 0x00,
   none                 = 0x00 + UNIT_OFFSET,
   hz                   = 0x01 + UNIT_OFFSET,
   time                 = 0x02 + UNIT_OFFSET,
   date                 = 0x03 + UNIT_OFFSET,
   centiCelsius         = 0x04 + UNIT_OFFSET,
   percentHumidity      = 0x05 + UNIT_OFFSET,
   voc                  = 0x06 + UNIT_OFFSET,
   permilPwm            = 0x07 + UNIT_OFFSET,
   _switch              = 0x08 + UNIT_OFFSET,
   version              = 0x09 + UNIT_OFFSET,    // To be removed
   softwareVersion      = 0x09 + UNIT_OFFSET,
   hardwareRevision     = 0x0A + UNIT_OFFSET,
   milliVolt            = 0x0B + UNIT_OFFSET,
   durationSeconds      = 0x0C + UNIT_OFFSET,    // Seconds
   pressure             = 0x0D + UNIT_OFFSET,
   command              = 0x0E + UNIT_OFFSET,
   timeStamp            = 0x0F + UNIT_OFFSET,
   milliAmpere          = 0x10 + UNIT_OFFSET,
   address              = 0x11 + UNIT_OFFSET,
   nodeId               = 0x12 + UNIT_OFFSET,
   tdtObject            = 0x13 + UNIT_OFFSET,
   number               = 0x14 + UNIT_OFFSET,   // Serial number, Article ID etc.
   numberHex            = 0x15 + UNIT_OFFSET,
   percentQuality       = 0x16 + UNIT_OFFSET,
   timeStampDelta       = 0x17 + UNIT_OFFSET,
   article              = 0x18 + UNIT_OFFSET,
   powerSwitchFunction  = 0x19 + UNIT_OFFSET,
   index                = 0x1A + UNIT_OFFSET,
   deviceStatus         = 0x1B + UNIT_OFFSET,
   errorCode            = 0x1C + UNIT_OFFSET,
   eventCode            = 0x1D + UNIT_OFFSET,
   flags                = 0x1E + UNIT_OFFSET,
   room                 = 0x1F + UNIT_OFFSET,
   permilHumidity       = 0x20 + UNIT_OFFSET,
   logCode              = 0x21 + UNIT_OFFSET,
   capacityKB           = 0x22 + UNIT_OFFSET,
   durationDays         = 0x23 + UNIT_OFFSET,    // Days
   systemState          = 0x24 + UNIT_OFFSET,
};


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

   // system/device
   vbusLow              = 0x100 + EVENT_OFFSET,
   batteryLow           
= 0x101 + EVENT_OFFSET,
   couldNotReadEeprom   = 0x100 + EVENT_OFFSET,
   couldNotWriteEeprom  = 0x102 + EVENT_OFFSET,
   noRelease            = 0x103 + EVENT_OFFSET,

   // uart
   uartRxBufferOverflow = 0x200 + EVENT_OFFSET,
   uartTxBufferOverflow = 0x201 + EVENT_OFFSET,

   // I2C
   i2cIoError           = 0x300 + EVENT_OFFSET,

   // CAN
   canRxBufferFull      = 0x400 + EVENT_OFFSET,
   canTxBufferFull      = 0x401 + EVENT_OFFSET,

   lastEvent,
};

enum class ESystemState: uint32_t
{
   invalid,
   bootLoader,
   application
};

static_assert( (int)EEvent::lastEvent < 0x07FFFFFF, "EEvent last enum to high" );

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

      struct
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
