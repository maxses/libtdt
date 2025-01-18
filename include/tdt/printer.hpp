#ifndef TDT_PRINTER_EXT_HPP
#define TDT_PRINTER_EXT_HPP
//-----------------------------------------------------------------------------
//
//
//
//-----------------------------------------------------------------------------


#include <tdt/message.hpp>
#include <tdt/gen/objects.hpp>
#include <tdt/gen/printerBase.hpp>
#include <stdio.h>               // snprintf
#include <QTextStream>
#include <QMap>
#include <math.h>

#if defined USE_LEPTO
   #include <lepto/events.h>
   #include <lepto/log.h>
   #include <lepto/time.h>        // SEC_PER_DAY
#endif

#define ENUM_MAP( a, e ) { a::e, "" #e }
#define ENUM_MAP_TEXT( a, e, t ) { a::e, t }
// #define EID( id, subId ) ( ( id << 4 ) | subId )


namespace Tdt
{


class CPrinter: public CPrinterBase
{
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
      const QMap< int, const char *> m_eventMap{
          ENUM_MAP( (int)Tdt::EEvent, wannaSleepStart ),
          ENUM_MAP( (int)Tdt::EEvent, wannaSleepAll ),
          ENUM_MAP( (int)Tdt::EEvent, wannaSleepRoom0 ),
          ENUM_MAP( (int)Tdt::EEvent, wannaSleepRoom1 ),
          ENUM_MAP( (int)Tdt::EEvent, wannaSleepEnd ),
      };
      #endif
      
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
      
      CPrinter( )
      {
      };
       
      QString printValue( const Tdt::CMessage& msg ) const
      {
         QString s;
         QTextStream ts( &s );
         switch( msg.getFunctionCode() )
         {
            case Tdt::EFunctionCode::alert:
               s="Alert:";
               break;
            default:
               break;
         }

         switch( msg.getTdtUnit() )
         {
            case Tdt::EUnit::version:
            {
               Tdt::SValue::SSoftwareVersion sw
                        =msg.getTdtValue()->softwareVersion;
               ts       << ( sw.drift.bootloader ? "BL " : "")
                        << "v"
                        << sw.major << "." << sw.minor << "." << sw.patch
                        << "-" << sw.drift.distance
                        << ( sw.drift.dirty ? "+" : "");
               break;
            }
            case Tdt::EUnit::durationSeconds:
            {
               int secs = (int)msg.getTdtValueUInt();
               if ( secs < 100 )
               {
                  ts << msg.getTdtValue()->_int << " s";
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
               Tdt::SValue::STime t=msg.getTdtValue()->time;
               ts << t.hour << ":" << t.min << ":" << t.sec;
               break;
            }
            case Tdt::EUnit::percentHumidity:
            {
               ts << ( ( msg.getTdtValue()->_int / 10 ) / 100.0) << " %";
               break;
            }
            case Tdt::EUnit::_switch:
            {
               ts << ( msg.getTdtValue()->_bool ? "ON" : "OFF" );
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
               unsigned int value=msg.getTdtValue()->_uint;
               ts << value << " %; ";
               for(int i1=0; i1<sizeof(ranges)/sizeof(ranges[0]); i1++)
               {
                  if( value >= ranges[i1].value )
                  {
                     ts << ranges[i1].text;
                     break;
                  }
               }
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
               if( map.contains(msg.getTdtValue()->_uint) )
               {
                  ts << map[msg.getTdtValue()->_uint];
               }
               else
               {
                  ts << "UK Article: " << msg.getTdtValue()->_uint;
               }
               break;
            }
            case Tdt::EUnit::powerSwitchFunction:
            {
               Tdt::EObject object=
                   (Tdt::EObject)(msg.getTdtValue()->_uint
                                       + (int)Tdt::EObject::powerSwitchStart);
               if( m_objectMap.contains(object) )
               {
                  ts << m_objectMap[object];
               }
               else
               {
                  ts << "UK Object (via Func): " << (int)object;
               }
               break;
            }
            case Tdt::EUnit::index:
            {
               ts << "[" << msg.getTdtValue()->_uint << "]";
               break;
            }
            case Tdt::EUnit::deviceStatus:
            {
               switch( msg.getTdtValue()->_uint )
               {
                  // Brrr, There   friend class CTopic; should not be a depency to biwak;
                  // CHeartBeats "DeviceStatus" should be moved to lepto
                  case 0:
                     ts << "Good";
                     break;
                  case 1:
                     ts << "Warning";
                     break;
                  case 2 ... 3:
                     ts << "Error(" << msg.getTdtValue()->_uint << ")";
                     break;
                  case 5:
                     ts << "Calm";
                     break;
                  case 8:
                     ts << "Resetting";
                     break;
                  default:
                     ts << "UK:" << msg.getTdtValue()->_uint;
                     break;
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
            case Tdt::EUnit::room:
            {
               if( m_roomMap.contains( msg.getTdtValue()->_uint ) )
               {
                  ts << m_roomMap[ msg.getTdtValue()->_uint ];
               }
               else
               {
                  ts << "UK Article: " << msg.getTdtValue()->_uint;
               }
               break;
            }
            case Tdt::EUnit::capacity:
            {
               ts << (int)msg.getTdtValueUInt() / 1000 << " KB";
               break;
            }
            case Tdt::EUnit::systemState:
            {
               switch( msg.getTdtValue()->systemState )
               {
                  case Tdt::ESystemState::application:
                     ts << "Application";
                     break;
                  case Tdt::ESystemState::bootLoader:
                     ts << "Bootloader";
                     break;
                  case Tdt::ESystemState::reverseBootLoader:
                     ts << "Reverse Bootloader";
                     break;
                  default:
                     ts << "Unknown ("
                        << (uint32_t)msg.getTdtValue()->systemState
                        << ")";
                     break;
               }
               break;
            }
            default:
            {
               if( m_unitMap.contains( msg.getTdtUnit() ) )
               {
                  SUnitDesc desc=m_unitMap[ msg.getTdtUnit() ];
                  if( desc.format == 'x' )
                  {
                     ts.setIntegerBase(16);
                     ts << "0x" << (unsigned int)msg.getTdtValueUInt();
                  }
                  else
                  {
                     if( desc.decimalPower != 1 )
                     {
                        float t = msg.getTdtValue()->_int * ( pow(10.0, desc.decimalPower ) );
                        ts << t;
                     }
                     else
                     {
                        ts << (int)msg.getTdtValueUInt();
                     }
                  }
                  if( desc.postfix[0] )
                  {
                     ts << " " << QString::fromUtf8( desc.postfix );
                  }
               }
               else
               {
                  ts.setIntegerBase(16);
                  ts << "UK: Unit=0x" << (int)msg.getTdtUnit();
               }
               break;
            }
         }

         return(s);
      };

      const char *getUnitString( const Tdt::CMessage& msg ) const
      {
         if( m_unitMap.contains( msg.getTdtUnit() ) )
         {
            return( m_unitMap[ msg.getTdtUnit() ].name );
         }
         return("-");
      }
      const char *getObjectString( const Tdt::CMessage& msg ) const
      {
         if( m_objectMap.contains( msg.getTdtObject() ) )
         {
            return( m_objectMap[ msg.getTdtObject() ] ); 
         }
         // Temporary solution
         static QString s;
         s=QString("? [0x%1]")
                 .arg((int)msg.getTdtObject(),0,16);
         return(qPrintable( s ) );
      }
      const char *getFunctionCodeString( const Tdt::CMessage& msg ) const
      {
         if( m_functionCodeMap.contains( msg.getFunctionCode() ) )
         {
            return( m_functionCodeMap[ msg.getFunctionCode() ] );
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


} // namespace Tdt


//---fin-----------------------------------------------------------------------
#endif // ? ! TDT_PRINTER_EXT_HPP
