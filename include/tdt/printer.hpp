#ifndef TDT_PRINTER_EXT_HPP
#define TDT_PRINTER_EXT_HPP
//-----------------------------------------------------------------------------
//
//
//
//-----------------------------------------------------------------------------


#include <tdt/message.hpp>
#include <tdt/topic.hpp>
#include <tdt/gen/objects.hpp>
#include <tdt/gen/printerBase.hpp>
#include <stdio.h>               // snprintf
#include <QTextStream>
#include <QMap>

#if defined USE_LEPTO
   #include <lepto/events.h>
   #include <lepto/log.h>
#endif

#define ENUM_MAP( a, e ) { a::e, "" #e }
#define ENUM_MAP_TEXT( a, e, t ) { a::e, t }
// #define EID( id, subId ) ( ( id << 4 ) | subId )

//using namespace Tdt;

class CCanTdtPrinter: public Tdt::CPrinterBase /* CPrinterBase */
{
   friend class CTopic;
      const Tdt::CMessage &m_message;
#if 0
      static QMap<Tdt::EObject, const char *> m_objectMap;
      const QMap<Tdt::EObject, const char *> m_objectMap__{
         ENUM_MAP( Tdt::EObject, error ),
          
         ENUM_MAP( Tdt::EObject, none ),
         ENUM_MAP_TEXT( Tdt::EObject, startSwitches, "disabled" ),
         ENUM_MAP( Tdt::EObject, ambientLightAll ),
         ENUM_MAP( Tdt::EObject, ambientLightLivingRoom ),
         ENUM_MAP( Tdt::EObject, ambientLightSleepingRoom ),
         ENUM_MAP( Tdt::EObject, ambientLightKitchen ),
         ENUM_MAP( Tdt::EObject, ambientLightOffice ),
         ENUM_MAP( Tdt::EObject, ambientLightBath1 ),
         ENUM_MAP( Tdt::EObject, ambientLightBath2 ),
         ENUM_MAP( Tdt::EObject, plantLight ),
         ENUM_MAP( Tdt::EObject, multimediaSwitch ),
         ENUM_MAP( Tdt::EObject, mainSwitch ),
         ENUM_MAP_TEXT( Tdt::EObject, endSwitches, "disabled" ),
         ENUM_MAP( Tdt::EObject, date ),
         ENUM_MAP( Tdt::EObject, time ),
         ENUM_MAP( Tdt::EObject, dummy ),
#if 0
         ENUM_MAP( Tdt::EObject, plantSensor ),
         ENUM_MAP_TEXT( Tdt::EObject, plantSensor0, "plantSensor[0]" ),
         ENUM_MAP_TEXT( Tdt::EObject, plantSensor1, "plantSensor[1]" ),
         ENUM_MAP_TEXT( Tdt::EObject, plantSensor2, "plantSensor[2]" ),
         ENUM_MAP_TEXT( Tdt::EObject, plantSensor3, "plantSensor[3]" ),
         ENUM_MAP_TEXT( Tdt::EObject, plantSensor4, "plantSensor[4]" ),
#endif
         ENUM_MAP( Tdt::EObject, firmwareVersion ),
         ENUM_MAP( Tdt::EObject, firmwareDate ),
         ENUM_MAP( Tdt::EObject, hardwareRevision ),
         ENUM_MAP( Tdt::EObject, hardwareDate ),
         ENUM_MAP( Tdt::EObject, humidity ),
         ENUM_MAP( Tdt::EObject, voc ),
         ENUM_MAP( Tdt::EObject, pressure ),

         ENUM_MAP( Tdt::EObject, temperature ),
         ENUM_MAP_TEXT( Tdt::EObject, temperature0, "temperature[0]" ),
         ENUM_MAP_TEXT( Tdt::EObject, temperature1, "temperature[1]" ),
         ENUM_MAP_TEXT( Tdt::EObject, temperature2, "temperature[2]" ),
         ENUM_MAP_TEXT( Tdt::EObject, temperature3, "temperature[3]" ),
         ENUM_MAP_TEXT( Tdt::EObject, temperature4, "temperature[4]" ),

         ENUM_MAP( Tdt::EObject, batteryLevel ),
         ENUM_MAP( Tdt::EObject, rtcDrift ),

         ENUM_MAP( Tdt::EObject, nodeId ),
         ENUM_MAP( Tdt::EObject, uidLow ),
         ENUM_MAP( Tdt::EObject, uidHigh ),
         ENUM_MAP( Tdt::EObject, iaq ),
         ENUM_MAP( Tdt::EObject, current ),
         ENUM_MAP( Tdt::EObject, currentAvg ),
         ENUM_MAP( Tdt::EObject, SupplyVoltage ),
         ENUM_MAP( Tdt::EObject, batteryLevel ),
         ENUM_MAP( Tdt::EObject, articleId ),
         ENUM_MAP( Tdt::EObject, serialNumber ),
         ENUM_MAP( Tdt::EObject, systemState ),
          
         ENUM_MAP( Tdt::EObject, timeStamp ),
         ENUM_MAP( Tdt::EObject, timeStampDrift ),
          
         ENUM_MAP( Tdt::EObject, foodTemperature ),
          
         ENUM_MAP_TEXT( Tdt::EObject, displayNode0, "displayNode[0]" ),
         ENUM_MAP_TEXT( Tdt::EObject, displayNode1, "displayNode[1]" ),
         ENUM_MAP_TEXT( Tdt::EObject, displayNode2, "displayNode[2]" ),
         ENUM_MAP_TEXT( Tdt::EObject, displayNode3, "displayNode[3]" ),
         ENUM_MAP_TEXT( Tdt::EObject, displayNode0ObjectOffset, "displayNode[0].objectOffset" ),
         ENUM_MAP_TEXT( Tdt::EObject, displayNode1ObjectOffset, "displayNode[1].objectOffset" ),
         ENUM_MAP_TEXT( Tdt::EObject, displayNode2ObjectOffset, "displayNode[2].objectOffset" ),
         ENUM_MAP_TEXT( Tdt::EObject, displayNode3ObjectOffset, "displayNode[3].objectOffset" ),
         
         ENUM_MAP_TEXT( Tdt::EObject, switchObjectOffset0, "switchObjectOffset[0]" ),
         ENUM_MAP_TEXT( Tdt::EObject, switchObjectOffset1, "switchObjectOffset[1]" ),
         ENUM_MAP_TEXT( Tdt::EObject, switchObjectOffset2, "switchObjectOffset[2]" ),
         ENUM_MAP_TEXT( Tdt::EObject, switchObjectOffset3, "switchObjectOffset[3]" ),
          
         ENUM_MAP_TEXT( Tdt::EObject, deviceStatus, "Device status" ),
         ENUM_MAP( Tdt::EObject, errorCode ),
         ENUM_MAP_TEXT( Tdt::EObject, eventCode, "Event code" ),
         ENUM_MAP( Tdt::EObject, flags ),
         ENUM_MAP( Tdt::EObject, bootCounter ),
         ENUM_MAP( Tdt::EObject, operatingTime ),
         ENUM_MAP( Tdt::EObject, shutdownTime ),
         ENUM_MAP( Tdt::EObject, releaseState ),
         ENUM_MAP( Tdt::EObject, fadeTime ),
         ENUM_MAP( Tdt::EObject, room ),
         ENUM_MAP_TEXT( Tdt::EObject, flashSize, "Flash" ),
         ENUM_MAP_TEXT( Tdt::EObject, chipId, "MCU ID" ),
         ENUM_MAP_TEXT( Tdt::EObject, eventCodeRepeated, "Log code R" ),
         ENUM_MAP_TEXT( Tdt::EObject, logCode, "log code" ),
      };
#endif
      const QMap<Tdt::EUnit, const char *> m_unitMap{
         ENUM_MAP( Tdt::EUnit, none ),
         ENUM_MAP( Tdt::EUnit, hz ),
         ENUM_MAP( Tdt::EUnit, time ),
         ENUM_MAP( Tdt::EUnit, date ),
         ENUM_MAP( Tdt::EUnit, centiCelsius ),
         ENUM_MAP( Tdt::EUnit, percentHumidity ),
         ENUM_MAP( Tdt::EUnit, voc ),
         ENUM_MAP( Tdt::EUnit, permilPwm ),
         ENUM_MAP( Tdt::EUnit, _switch ),
         ENUM_MAP( Tdt::EUnit, version ),
         ENUM_MAP( Tdt::EUnit, softwareVersion ),
         ENUM_MAP( Tdt::EUnit, hardwareRevision ),
         ENUM_MAP( Tdt::EUnit, milliVolt ),
         ENUM_MAP( Tdt::EUnit, durationSeconds ),
         ENUM_MAP( Tdt::EUnit, timeStamp ),
         ENUM_MAP( Tdt::EUnit, errorCode ),
         ENUM_MAP( Tdt::EUnit, eventCode ),
         ENUM_MAP( Tdt::EUnit, room ),
      };

      const QMap<Tdt::EFunctionCode, const char *> m_functionCodeMap{
         ENUM_MAP( Tdt::EFunctionCode, nmt ),
         ENUM_MAP( Tdt::EFunctionCode, alert ),
         ENUM_MAP( Tdt::EFunctionCode, reserved1 ),
         ENUM_MAP( Tdt::EFunctionCode, writeObject ),
         ENUM_MAP( Tdt::EFunctionCode, readObject ),
         ENUM_MAP( Tdt::EFunctionCode, sendObject ),
         ENUM_MAP( Tdt::EFunctionCode, dataBlob ),
         ENUM_MAP( Tdt::EFunctionCode, reSendObject ),
         //ENUM_MAP( Tdt::EFunctionCode, log ),
         ENUM_MAP( Tdt::EFunctionCode, max ),
		};

      #if 0
      const QMap< int, const char *> m_eventComponentMap{
         { CAN_TRANSMIT_BUFFER,     "CAN transmit buffer"},
         { RADIO_TRANSMIT_BUFFER,   "radio transmit buffer"},
      };
      #endif
      
      const QMap< int, const char *> m_eventMap{
         #if 0
         { BUFFER_FULL,     "buffer full"},
         #endif
          ENUM_MAP( (int)Tdt::EEvent, wannaSleepStart ),
          ENUM_MAP( (int)Tdt::EEvent, wannaSleepAll ),
          ENUM_MAP( (int)Tdt::EEvent, wannaSleepRoom0 ),
          ENUM_MAP( (int)Tdt::EEvent, wannaSleepRoom1 ),
          ENUM_MAP( (int)Tdt::EEvent, wannaSleepEnd ),
          ENUM_MAP( (int)Tdt::EEvent, vbusLow ),
          ENUM_MAP( (int)Tdt::EEvent, batteryLow ),
          ENUM_MAP( (int)Tdt::EEvent, i2cIoError ),
          ENUM_MAP_TEXT( (int)Tdt::EEvent, noRelease, "NO RELEASE!" ),
      };
      
      const QMap<int, const char*>m_roomMap{
          { 0x0, "none"},
          { 0x1, "all"},
          { 0x2, "living room"},
          { 0x3, "sleeping room"},
          { 0x4, "Kitchen"},
          { 0x5, "Office"},
          { 0x6, "Bath1"},
          { 0x7, "Bath2"},
      };
      
   public:
      
      CCanTdtPrinter( const Tdt::CMessage &message )
         :m_message( message )
      {
		};

		QString printValue( ) const
      {
			QString s;
         switch( m_message.getFunctionCode() )
         {
            case Tdt::EFunctionCode::alert:
               s="Alert:";
               break;
            default:
               break;
         }

         switch( m_message.getTdtUnit() )
         {
            case Tdt::EUnit::centiCelsius:
            {
               float t = m_message.getTdtValue()->_int / 100.0;
               QTextStream( &s ) << t <<  QString::fromUtf8(" °C");
               break;
            }
            case Tdt::EUnit::version:
            {
               QTextStream( &s )
                        << ( m_message.getTdtValue()->softwareVersion.drift.bootloader ? "BL " : "")
                        << "v"
                        << m_message.getTdtValue()->softwareVersion.major << "."
                        << m_message.getTdtValue()->softwareVersion.minor << "."
                        << m_message.getTdtValue()->softwareVersion.patch << "-"
                        << m_message.getTdtValue()->softwareVersion.drift.distance
                        << ( m_message.getTdtValue()->softwareVersion.drift.dirty ? "+" : "");
               break;
            }
            case Tdt::EUnit::hz:
            {
               QTextStream( &s ) << (int)(m_message.getTdtValue()->_int) << " Hz";
               break;
            }
            case Tdt::EUnit::durationSeconds:
            {
               int secs = (int)m_message.getTdtValueUInt();
               QTextStream ts( &s );
               
               if ( secs < 100 )
               {
                  ts << m_message.getTdtValue()->_int << " s";
               }
               else
               {
                  int days = secs / SEC_PER_DAY;
                  secs-=days * SEC_PER_DAY;
                  int hours =  secs / SEC_PER_HOUR;
                  secs-=hours * SEC_PER_HOUR;
                  int minutes =  secs / SEC_PER_MIN;
                  secs-=minutes * SEC_PER_MIN;
                  
                  if(days)
                  {
                     ts << days << "d ";
                  }
                  ts << hours << ":" << minutes << ":" << secs << "s";
               }
               break;
            }
            case Tdt::EUnit::time:
				{
               QTextStream( &s )
                        << m_message.getTdtValue()->time.hour << ":"
                        << m_message.getTdtValue()->time.min << ":"
                        << m_message.getTdtValue()->time.sec;
					break;
				}
            case Tdt::EUnit::percentHumidity:
            {
               QTextStream( &s )
                     << ( ( m_message.getTdtValue()->_int / 10 ) / 100.0) << " %";
               break;
            }
            case Tdt::EUnit::pressure:
            {
               QTextStream( &s ) << m_message.getTdtValue()->_int << " hPa";
               break;
            }
            case Tdt::EUnit::voc:
            {
               QTextStream( &s ) << m_message.getTdtValue()->_int << " Ohm";
               break;
            }
            case Tdt::EUnit::permilPwm:
            {
               QTextStream( &s ) << m_message.getTdtValue()->_int << " ‰ PWM";
               break;
            }
            case Tdt::EUnit::_switch:
            {
               QTextStream( &s ) << (m_message.getTdtValue()->_bool ? "ON" : "OFF");
               break;
            }
            case Tdt::EUnit::milliVolt:
            {
               QTextStream( &s ) << m_message.getTdtValue()->_int << " mV";
               break;
            }
            case Tdt::EUnit::milliAmpere:
            {
               QTextStream( &s ) << m_message.getTdtValue()->_int << " mA";
               break;
            }
            case Tdt::EUnit::numberHex:
            {
               QTextStream ts( &s );
               ts.setIntegerBase(16);
               ts << "0x" << m_message.getTdtValue()->_uint;
               break;
            }
            case Tdt::EUnit::number:
            case Tdt::EUnit::nodeId:
            {
               QTextStream( &s ) << m_message.getTdtValue()->_uint;
               break;
            }
            case Tdt::EUnit::percentQuality:
            {
               const struct {int value; const char *text;} ranges[]=
               {
                  { 90, "Good" },
                  { 70, "Moderate" },
                  { 65, "Unhealthy for Sensitive Groups" },
                  { 60, "Unhealthy" },
                  { 40, "Very Unhealthy" },
                  {  0, "Hazardous" },
               };
               unsigned int value=m_message.getTdtValue()->_uint;
               QTextStream( &s ) << value << " %; ";
               for(int i1=0; i1<sizeof(ranges)/sizeof(ranges[0]); i1++)
               {
                  if( value >= ranges[i1].value )
                  {
                     QTextStream( &s ) << ranges[i1].text;
                     break;
                  }
               }
               break;
            }
            case Tdt::EUnit::timeStamp:
            {
               QTextStream( &s ) << m_message.getTdtValue()->timestamp << "s";
               break;
            }
            case Tdt::EUnit::timeStampDelta:
            {
               QTextStream( &s ) << m_message.getTdtValue()->timestampdelta << "s";
               break;
            }
            case Tdt::EUnit::article:
            {
               QMap<int, const char*>map{
                  { 0x50, "CAN/IO"},
                  { 0x51, "CANBridge"},
                  { 0x52, "Rufa"},
                  { 0x60, "CANDis"},
                  { 0x80, "Sensemux"},
                  { 0x90, "Minutnik-EInk"},
                  { 0xA0, "CANRec"},
                  { 0xB0, "CANSwitch"},
                  { 0xD0, "Main-Switch"},
               };
               if( map.contains(m_message.getTdtValue()->_uint) )
               {
                  QTextStream( &s ) << map[m_message.getTdtValue()->_uint];
               }
               else
               {
                  QTextStream( &s ) << "UK Article: " << m_message.getTdtValue()->_uint;
               }
               break;
            }
            case Tdt::EUnit::powerSwitchFunction:
            {
               /*
               QMap<int, const char*>map{
                                           { 0x0, "Disabled (0)"},
                                           { 0x1, "Ambient light(1)"},
                                           { 0x2, "Plant light(2)"},
                                           { 0x3, "Media(3)"},
                                           };
               */
               Tdt::EObject object=
                   (Tdt::EObject)(m_message.getTdtValue()->_uint
                                       + (int)Tdt::EObject::powerSwitchStart);
               if( m_objectMap.contains(object) )
               {
                  QTextStream( &s ) << m_objectMap[object];
               }
               else
               {
                  QTextStream( &s ) << "UK Object (via Func): " << (int)object;
               }
               break;
            }
            case Tdt::EUnit::index:
            {
               QTextStream( &s ) << "[" << m_message.getTdtValue()->_uint << "]";
               break;
            }
            case Tdt::EUnit::deviceStatus:
            {
               switch( m_message.getTdtValue()->_uint )
               {
                  // Brrr, There should not be a depency to biwak;
                  // CHeartBeats "DeviceStatus" should be moved to lepto
                  case 0:
                     QTextStream( &s ) << "Good";
                     break;
                  case 1:
                     QTextStream( &s ) << "Warning";
                     break;
                  case 2 ... 3:
                     QTextStream( &s ) << "Error(" << m_message.getTdtValue()->_uint << ")";
                     break;
                  case 5:
                     QTextStream( &s ) << "Calm";
                     break;
                  case 8:
                     QTextStream( &s ) << "Resetting";
                     break;
                  default:
                     QTextStream( &s ) << "UK:" << m_message.getTdtValue()->_uint;
                     break;
               }
               break;
            }
            case Tdt::EUnit::errorCode:
            {
               switch( m_message.getTdtValue()->_uint )
               {
                  default:
                  {
                     QTextStream ts( &s );
                     ts.setIntegerBase(16);
                     ts << "0x" << (int)m_message.getTdtValueUInt();
                     break;
                  }
               }
               break;
            }
            #if 0
            case Tdt::EUnit::eventCode:
            {
               QString component;
               QString eventString;
               unsigned int eventCode=m_message.getTdtValue()->_uint;
               
               component = m_eventComponentMap[ eventCode & COMPONENT_MASK ];
               eventString = m_eventMap[ eventCode & EVENT_MASK ];
               if( component.isEmpty() )
               {
                  component = QString( "C=0x%1" ).arg(eventCode, 0, 16);
               }
               if( eventString.isEmpty() )
               {
                  eventString = QString( "0x%1" ).arg(eventCode, 0, 16);
               }
               QTextStream ts( &s );
               ts << component << " : " << eventString;
               break;
            }
            #endif
            case Tdt::EUnit::flags:
            {
               QTextStream ts( &s );
               ts.setIntegerBase(16);
               ts << "0x" << (int)m_message.getTdtValueUInt();
               break;
            }
            case Tdt::EUnit::room:
            {
               if( m_roomMap.contains(m_message.getTdtValue()->_uint) )
               {
                  QTextStream( &s ) << m_roomMap[m_message.getTdtValue()->_uint];
               }
               else
               {
                     QTextStream( &s ) << "UK Article: " << m_message.getTdtValue()->_uint;
               }
               break;
            }
            case Tdt::EUnit::permilHumidity:
            {
               QTextStream ts( &s );
               ts << (int)m_message.getTdtValueUInt() << " ‰";
               break;
            }
            case Tdt::EUnit::capacityKB:
            {
               QTextStream ts( &s );
               ts << (int)m_message.getTdtValueUInt() << " KB";
               break;
            }
            case Tdt::EUnit::systemState:
            {
               QTextStream ts( &s );
               switch( m_message.getTdtValue()->systemState )
               {
                  case Tdt::ESystemState::application:
                     ts << "Application";
                     break;
                  case Tdt::ESystemState::bootLoader:
                     ts << "Bootloader";
                     break;
                  default:
                     ts << "Unknown ("
                        << (uint32_t)m_message.getTdtValue()->systemState
                        << ")";
                     break;
               }
               break;
            }
            default:
            {
               QTextStream ts( &s );
               ts.setIntegerBase(16);
               ts << "UK: Unit=0x" << (int)m_message.getTdtUnit();
               break;
            }
         }

			return(s);
		};

      const char *getUnitString() const
      {
			if( m_unitMap.contains( m_message.getTdtUnit() ) )
			{
				return( m_unitMap[ m_message.getTdtUnit() ] );
			}
			return("-");
      }
      const char *getObjectString() const
      {
			if( m_objectMap.contains(m_message.getTdtObject()) )
			{
				return( m_objectMap[ m_message.getTdtObject() ] ); 
			}
         return("-");
      }
      const char *getFunctionCodeString() const
      {
			if( m_functionCodeMap.contains(m_message.getFunctionCode()) )
			{
				return( m_functionCodeMap[ m_message.getFunctionCode() ] );
			}
			return("-");
		}
      
      const QMap<Tdt::EObject, const char *>& getObjectMap() const
      {
         return(m_objectMap);
      }
      const QMap<int, const char *>& getRoomMap() const
      {
         return(m_roomMap);
      }
};

//extern CTopic topicPlants;extern CTopic topicPlants;


//---fin-----------------------------------------------------------------------
#endif // ? ! TDT_PRINTER_EXT_HPP
