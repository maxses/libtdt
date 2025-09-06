#ifndef TDT_PRINTER_EXT_HPP
#define TDT_PRINTER_EXT_HPP
/**---------------------------------------------------------------------------
 *
 * @file    printer.hpp
 * @brief   Generate text fragments from tdt messages
 *
 * Those text strings can be used by user interfaces on host PC.
 * For text fragments to be used on target microcontroller, see "printer_biwak.hpp".
 * 
 * @date      20241214
 * @author    Maximilian Seesslen <mes@seesslen.net>
 * @copyright SPDX-License-Identifier: Apache-2.0
 *
 *--------------------------------------------------------------------------*/


/*--- Includes -------------------------------------------------------------*/


#include <tdt/message.hpp>
#include <tdt/gen/objects.hpp>
#include <tdt/gen/printerBase.hpp>
#include <stdio.h>               // snprintf
#include <QTextStream>
#include <QMap>
#include <math.h>

#if defined USE_LEPTO
   // #include <lepto/events.h>
   #include <lepto/log.h>
   #include <lepto/units.h>        // SEC_PER_DAY
#endif

#define ENUM_MAP( a, e ) { a::e, "" #e }
#define ENUM_MAP_TEXT( a, e, t ) { a::e, t }
// #define EID( id, subId ) ( ( id << 4 ) | subId )


/*--- Declarations ---------------------------------------------------------*/


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
         ENUM_MAP( Tdt::EFunctionCode, ackDataBlob ),
         //ENUM_MAP( Tdt::EFunctionCode, log ),
         ENUM_MAP( Tdt::EFunctionCode, max ),
      };

   public:
      
      CPrinter( )
      {
      };
      
      QString printValue( const Tdt::CMessage& msg ) const
      {
         return( printValue( msg.getFunctionCode(), msg.getTdtUnit(), *msg.getTdtValue() ) );
      };
      
      QString printValue( EFunctionCode functionCode, EUnit unit, const SValue& value ) const;
      QString printValueGeneric( EFunctionCode functionCode, EUnit unit, const SValue& value, const SUnitDesc& unitDesc ) const;

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
};


} // namespace Tdt


/*--- Fin ------------------------------------------------------------------*/
#endif // ? ! TDT_PRINTER_EXT_HPP
