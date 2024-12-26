//----------------------------------------------------------------------------
///
///   \file    unit.cpp
///   \brief   Carries description for TDT-Unit
///
///            Print the enums
///
///   \author  Maximilian Seesslen <mes@seesslen.net>
///
//----------------------------------------------------------------------------


//--- Includes ---------------------------------------------------------------


#include <unit.hpp>
#include <profile.hpp>


//--- Implementation----------------------------------------------------------


QString CUnit::enumString()
{
   QString str( QString("      %1").arg( nameToEnum() ) );
   QString strNewLine;
   appendObjectNumber(str, m_unit);
   str=str.leftJustified( 55, ' ' );
   str+=QString( "// %1").arg(m_desc);
   
   return( str );
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
}

void CUnit::appendObjectNumber( QString& str, int objectNumber ) const
{
   str=str.leftJustified( 35, ' ' );
   str+=QString("= 0x%1 + 0x%2,").arg(m_profile.getBase(), 4, 16, QChar('0') )
              .arg( objectNumber, 4, 16, QChar('0') );
   return;
}

QString CUnit::nameToEnum()
{
   #if 0    // like "ePowerLine"
      QString enumName="e" + m_name;
      enumName.replace( 1, 1, enumName.at(1).toUpper() );
   #else    // like "powerLine"
      QString enumName=m_name;
      enumName.replace( 0, 1, enumName.at(0).toLower() );
   #endif
   
   int pos;
   while( ( pos=enumName.indexOf(' ') ) >= 0 )
   {
      enumName.removeAt( pos );
      enumName.replace( pos, 1, enumName.at(pos).toUpper() );
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
   str+=QString(", %1 " ).arg( m_decimalPower );
   str+="} },";
   str=str.leftJustified( 60, ' ' );
   str+=QString( "// %1").arg(m_desc);
   
   return( str );
}


//--- Fin. -------------------------------------------------------------------
