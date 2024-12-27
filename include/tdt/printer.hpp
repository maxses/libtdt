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

#if defined USE_LEPTO
   #include <lepto/events.h>
   #include <lepto/log.h>
   #include <lepto/time.h>        // SEC_PER_DAY
#endif

#define ENUM_MAP( a, e ) { a::e, "" #e }
#define ENUM_MAP_TEXT( a, e, t ) { a::e, t }
// #define EID( id, subId ) ( ( id << 4 ) | subId )


class CCanTdtPrinter: public Tdt::CPrinterBase
{
      Tdt::CMessage m_message;

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
      
      CCanTdtPrinter( )
      {
      };
       
      void setMessage( const Tdt::CMessage &message )
      {
         m_message=message;
      };
      
      QString printValue( ) const
      {
         QString s;
         QTextStream ts( &s );
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
            case Tdt::EUnit::version:
            {
               Tdt::SValue::SSoftwareVersion sw
                        =m_message.getTdtValue()->softwareVersion;
               ts       << ( sw.drift.bootloader ? "BL " : "")
                        << "v"
                        << sw.major << "." << sw.minor << "." << sw.patch
                        << "-" << sw.drift.distance
                        << ( sw.drift.dirty ? "+" : "");
               break;
            }
            case Tdt::EUnit::durationSeconds:
            {
               int secs = (int)m_message.getTdtValueUInt();
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
               Tdt::SValue::STime t=m_message.getTdtValue()->time;
               ts << t.hour << ":" << t.min << ":" << t.sec;
               break;
            }
            case Tdt::EUnit::percentHumidity:
            {
               ts << ( ( m_message.getTdtValue()->_int / 10 ) / 100.0) << " %";
               break;
            }
            case Tdt::EUnit::_switch:
            {
               ts << (m_message.getTdtValue()->_bool ? "ON" : "OFF");
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
               if( map.contains(m_message.getTdtValue()->_uint) )
               {
                  ts << map[m_message.getTdtValue()->_uint];
               }
               else
               {
                  ts << "UK Article: " << m_message.getTdtValue()->_uint;
               }
               break;
            }
            case Tdt::EUnit::powerSwitchFunction:
            {
               Tdt::EObject object=
                   (Tdt::EObject)(m_message.getTdtValue()->_uint
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
               ts << "[" << m_message.getTdtValue()->_uint << "]";
               break;
            }
            case Tdt::EUnit::deviceStatus:
            {
               switch( m_message.getTdtValue()->_uint )
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
                     ts << "Error(" << m_message.getTdtValue()->_uint << ")";
                     break;
                  case 5:
                     ts << "Calm";
                     break;
                  case 8:
                     ts << "Resetting";
                     break;
                  default:
                     ts << "UK:" << m_message.getTdtValue()->_uint;
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
               if( m_roomMap.contains(m_message.getTdtValue()->_uint) )
               {
                  ts << m_roomMap[m_message.getTdtValue()->_uint];
               }
               else
               {
                  ts << "UK Article: " << m_message.getTdtValue()->_uint;
               }
               break;
            }
            case Tdt::EUnit::capacity:
            {
               ts << (int)m_message.getTdtValueUInt() / 1000 << " KB";
               break;
            }
            case Tdt::EUnit::systemState:
            {
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
               if( m_unitMap.contains( m_message.getTdtUnit() ) )
               {
                  SUnitDesc desc=m_unitMap[ m_message.getTdtUnit() ];
                  if( desc.format == 'x' )
                  {
                     ts.setIntegerBase(16);
                     ts << "0x" << (int)m_message.getTdtValueUInt();
                  }
                  else
                  {
                     if( desc.decimalPower != 1 )
                     {
                        float t = m_message.getTdtValue()->_int * ( pow(10.0, desc.decimalPower ) );
                        ts << t;
                     }
                     else
                     {
                        ts << (int)m_message.getTdtValueUInt();
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
                  ts << "UK: Unit=0x" << (int)m_message.getTdtUnit();
               }
               break;
            }
         }

         return(s);
      };

      const char *getUnitString() const
      {
         if( m_unitMap.contains( m_message.getTdtUnit() ) )
         {
            return( m_unitMap[ m_message.getTdtUnit() ].name );
         }
         return("-");
      }
      const char *getObjectString() const
      {
         if( m_objectMap.contains(m_message.getTdtObject()) )
         {
            return( m_objectMap[ m_message.getTdtObject() ] ); 
         }
         // Temporary solution
         static QString s;
         s=QString("? [0x%1]")
                 .arg((int)m_message.getTdtObject(),0,16);
         return(qPrintable( s ) );
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


//---fin-----------------------------------------------------------------------
#endif // ? ! TDT_PRINTER_EXT_HPP
