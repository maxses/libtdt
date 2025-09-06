/**---------------------------------------------------------------------------
 *
 * @file    unit.cpp
 * @brief   Carries description for TDT-Unit
 *
 * Print the enums.
 *
 * @date      20241003
 * @author    Maximilian Seesslen <mes@seesslen.net>
 * @copyright SPDX-License-Identifier: Apache-2.0
 *
 *--------------------------------------------------------------------------*/


/*--- Includes -------------------------------------------------------------*/


#include <unit.hpp>
#include <profile.hpp>
#include <QJsonArray>


/*--- Implementation -------------------------------------------------------*/


QString CUnit::enumString()
{
   QString str( QString("   %1").arg( nameToEnum() ) );
   QString strNewLine;
   appendObjectNumber(str, m_unit);
   str=str.leftJustified( 55, ' ' );
   str+=QString( "// %1").arg(m_desc);
   
   return( str );
}


void CUnit::writeEnumTypeEnums( QTextStream& s )
{
   if( m_format == "E" )
   {
      QString enumName=nameToEnum();
      enumName.replace( 0, 1, enumName.at(0).toUpper() );

      s << "\nenum class E" << enumName << "\n{\n";
      for( auto e: m_values.keys() )
      {
         QString enumName=nameToEnum( m_values[e] );
         s << "   " << enumName << " = " << e << ",\n";
      }
      s << "};\n";
   }
   
   return;
}


void CUnit::parse( QJsonObject& o )
{
   if( o.contains("decimalPower") )
   {
      m_decimalPower=( o["decimalPower"].toString().toInt(nullptr, 0) );
   }
   
   if( o.contains("postfix") )
   {
      m_postfix=o["postfix"].toString();
   }
   
   if( o.contains("format") )
   {
      m_format=o["format"].toString();
   }

   if( m_format == "E" )
   {
      auto values=o["values"].toObject();
      for( auto value : values.keys() )
      {
         int numericValue=values.value( value ).toString().toInt(0, 0);
         qWarning() << "Key: " << value << ": " << values.value( value ).toString().toInt(0, 0)
                       << "/" << values.value( value ).toInt(0);
         m_values[ numericValue ] = value;
      }
   }
}


void CUnit::appendObjectNumber( QString& str, int objectNumber ) const
{
   str=str.leftJustified( 35, ' ' );
   str+=QString("= 0x%1 + 0x%2,").arg(m_profile.getUnitsBase(), 4, 16, QChar('0') )
              .arg( objectNumber, 4, 16, QChar('0') );
   return;
}


QString CUnit::nameToEnum()
{
   return( nameToEnum( m_name ) );
}


QString CUnit::nameToEnum( const QString& name ) /* static */ 
{
   #if 0    // like "ePowerLine"
      QString enumName="e" + m_name;
      enumName.replace( 1, 1, enumName.at(1).toUpper() );
   #else    // like "powerLine"
      QString enumName=name;
      enumName.replace( 0, 1, enumName.at(0).toLower() );
   #endif
   
   int pos;
   while( ( pos=enumName.indexOf(' ') ) >= 0 )
   {
      enumName.replace( pos, 2, enumName.at(pos+1).toUpper() );
   }
   
   // Just be sure first char is lowercase
   enumName.replace( 0, 1, enumName.at(0).toLower() );
   
   return(enumName);
}


QString CUnit::printerString()
{
   QString str( QString("      { %1%2,  ").arg( nsPrefix() ).arg( nameToEnum() ) );
   str=str.leftJustified( 20, ' ' );
   str+=QString("{ \"%1\" " ).arg( m_name );
   str+=QString(", \"%1\" " ).arg( m_postfix );
   str+=QString(", '%1' " ).arg( m_format.isEmpty() ? " " : m_format );
   str+=QString(", %1 " ).arg( m_decimalPower );

   // Print enum values
   if( m_values.size() )
   {
      str+=QString(", \n            {\n");
      for( auto value : m_values.keys() )
      {
         str+=QString("                  {%1, \"%2\" }, \n" ).arg( value ).arg( m_values[value] );
      }
      str+=QString("            } ");
   }

   str+="} }, ";
   str=str.leftJustified( 60, ' ' );
   str+=QString( "// %1").arg(m_desc);
   
   return( str );
}


/*--- Fin ------------------------------------------------------------------*/
