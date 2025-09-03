#ifndef TDT_PRINTER_BIWAK_HPP
#define TDT_PRINTER_BIWAK_HPP
/**---------------------------------------------------------------------------
 *
 * @file    printer_biwak.hpp
 * @brief   Generate text fragments from tdt messages
 *
 * Those text strings can be used on target MCU.
 * For text fragments to be used on host pc, see "printer.hpp".
 * 
 * @date      20241214
 * @author    Maximilian Seesslen <mes@seesslen.net>
 * @copyright SPDX-License-Identifier: Apache-2.0
 *
 *--------------------------------------------------------------------------*/


/*--- Includes -------------------------------------------------------------*/


#include <biwak/canMessage.hpp>
#include <lepto/log.h>
#include <stdio.h>               // snprintf
#include <tdt/message.hpp>
//#include <QMap>

#define ENUM_MAP( a, e ) { a::e, "" #e }

struct SPrinterObject{ Tdt::EObject object; const char *text;};

class CCanTdtPrinter
{
      const Tdt::CMessage &m_message;
      //QMap<Tdt::EObject, const char *>
      //const char *m_objectMap[int]{
      static struct SPrinterObject m_objectMap[];

   public:

   public:

      CCanTdtPrinter( const Tdt::CMessage &message)
         :m_message( message )
      {
      }
      void snprintValue(char *buf, int len) const
      {
         switch( m_message.getTdtUnit() )
         {
            case Tdt::EUnit::centiCelsius:
            {
               int temp=m_message.getTdtValue()->_int;
               int temp1=temp/100;
               // '\xD2'"\xF8"
               snprintf(buf, len, "%2d.%02d" "\xD2" "C", temp1, temp-(temp1*100) );
               break;
            }
            case Tdt::EUnit::version:
            {
               snprintf(buf, len, "v%d.%d.%d-%d%s"
                        , m_message.getTdtValue()->softwareVersion.major
                        , m_message.getTdtValue()->softwareVersion.minor
                        , m_message.getTdtValue()->softwareVersion.patch
                        , m_message.getTdtValue()->softwareVersion.drift.distance
                        , m_message.getTdtValue()->softwareVersion.drift.dirty ? "+" : "");
               break;
            }
            case Tdt::EUnit::hz:
               snprintf(buf, len, "%6d HZ", (int)m_message.getTdtValue()->_int );
               break;
            case Tdt::EUnit::durationSeconds:
            {
               snprintf(buf, len, "%d s", (int)m_message.getTdtValue()->_int );
               break;
            }
            case Tdt::EUnit::nodeId:
            {
               snprintf(buf, len, "%d", (int)m_message.getTdtValue()->_uint );
               break;
            }
            default:
               snprintf(buf, len, "UK: Unit=0x%X", (int)m_message.getTdtUnit());
               break;
         }
      }
      const char *getUnitString() const
      {
         switch(m_message.getTdtUnit())
         {
            case Tdt::EUnit::version:
               return("FW");
               break;
            case Tdt::EUnit::centiCelsius:
               return("TEMP");
               break;
            case Tdt::EUnit::hz:
               return("HZ");
               break;
            default:
               break;
         }
         return("-");
      }
      const char *getObjectString() const;
      const char *getFunctionCodeString() const
      {
         switch( m_message.getFunctionCode() )
         {
            case Tdt::EFunctionCode::nmt:
               return("NMT");
               break;
            case Tdt::EFunctionCode::alert:
               return("ALRT");
               break;
            case Tdt::EFunctionCode::writeObject:
               return("WO");
               break;
            case Tdt::EFunctionCode::readObject:
               return("RO");
               break;
            case Tdt::EFunctionCode::sendObject:
               return("SO");
               break;
            default:
               break;
         }
         return("-");
      }
};

struct SPrinterObject CCanTdtPrinter::m_objectMap[]{
   /*
   ENUM_MAP( Tdt::EObject, none ),
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
   ENUM_MAP( Tdt::EObject, date ),
   ENUM_MAP( Tdt::EObject, time ),
   ENUM_MAP( Tdt::EObject, dummy ),

   ENUM_MAP( Tdt::EObject, plantSensor ),
   ENUM_MAP( Tdt::EObject, plantSensor0 ),
   ENUM_MAP( Tdt::EObject, plantSensor1 ),
   ENUM_MAP( Tdt::EObject, plantSensor2 ),
   ENUM_MAP( Tdt::EObject, plantSensor3 ),
   ENUM_MAP( Tdt::EObject, plantSensor4 ),

   ENUM_MAP( Tdt::EObject, firmwareVersion ),
   ENUM_MAP( Tdt::EObject, firmwareDate ),
   ENUM_MAP( Tdt::EObject, hardwareRevision ),
   ENUM_MAP( Tdt::EObject, hardwareDate ),
   ENUM_MAP( Tdt::EObject, humidity ),
   ENUM_MAP( Tdt::EObject, voc ),

   ENUM_MAP( Tdt::EObject, temperature ),
   ENUM_MAP( Tdt::EObject, temperature0 ),
   ENUM_MAP( Tdt::EObject, temperature1 ),
   ENUM_MAP( Tdt::EObject, temperature2 ),
   ENUM_MAP( Tdt::EObject, temperature3 ),
   ENUM_MAP( Tdt::EObject, temperature4 ),

   ENUM_MAP( Tdt::EObject, batteryLevel ),
   ENUM_MAP( Tdt::EObject, rtcDrift ),
    */
};


const char *CCanTdtPrinter::getObjectString() const
{
   for(int i1=0; i1<(int)(sizeof(m_objectMap)/sizeof(m_objectMap[0])); i1++)
   {
      if( m_objectMap[0].object == m_message.getTdtObject() )
      {
         return( m_objectMap[0].text );
      }
   }
   return("-");
}


/*--- Fin ------------------------------------------------------------------*/
#endif // ? ! TDT_PRINTER_BIWAK_HPP
