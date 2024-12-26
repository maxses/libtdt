//----------------------------------------------------------------------------
///
///   \file    object.cpp
///   \brief   Carries description for TDT-Object
///
///            Print the enums
///
///   \author  Maximilian Seesslen <mes@seesslen.net>
///
//----------------------------------------------------------------------------


//--- Includes ---------------------------------------------------------------


#include <object.hpp>
#include <profile.hpp>


//--- Implementation----------------------------------------------------------


QString CObject::enumString()
{
   QString str( QString("      %1").arg( nameToEnum() ) );
   QString strNewLine;
   appendObjectNumber(str, m_object);
   str=str.leftJustified( 55, ' ' );
   str+=QString( "// %1").arg(m_desc);
   
   if( m_size )
   {
      str+="\n";
      strNewLine=QString("      %1Start").arg( nameToEnum() );
      appendObjectNumber(strNewLine, m_object);
      str+=strNewLine;
      str+="\n";
      
      for(int i1=0; i1<m_size; i1++)
      {
         strNewLine=QString("      %1%2").arg( nameToEnum() ).arg(i1);
         appendObjectNumber(strNewLine, m_object+i1);
         str+=strNewLine;
         str+="\n";
      }
      strNewLine=QString("      %1Max").arg( nameToEnum() );
      appendObjectNumber(strNewLine, m_object+m_size-1);
      str+=strNewLine;
   }
   
   return( str );
}

void CObject::parse( QJsonObject& o )
{
   if( o.contains("size") )
   {
      setSize( o["size"].toString().toInt(nullptr, 0) );
   }
}

void CObject::appendObjectNumber( QString& str, int objectNumber ) const
{
   str=str.leftJustified( 35, ' ' );
   str+=QString("= 0x%1 + 0x%2,").arg(m_profile.getBase(), 4, 16, QChar('0') )
              .arg( objectNumber, 4, 16, QChar('0') );
   return;
}

QString CObject::nameToEnum()
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
   
   if( m_profileInEnum )
   {
      QString profile=m_profile.getName();
      if( profile.length() )
      {
         profile.replace( 0, 1, profile.at(0).toUpper() );
      }
      enumName.replace( 0, 1, enumName.at(0).toUpper() );
      enumName.insert( 0 /* 1 if 'eName' is used' */ , profile);
   }
   
   // Just be sure first char is lowercase
   enumName.replace( 0, 1, enumName.at(0).toLower() );
   
   return(enumName);
}


QString CObject::printerString()
{
   QString str( QString("      { %1%2,  ").arg( nsPrefix() ).arg( nameToEnum() ) );
   str=str.leftJustified( 20, ' ' );
   str+=QString(" \"%1\" }," ).arg( m_name);
   str=str.leftJustified( 60, ' ' );
   str+=QString( "// %1").arg(m_desc);
   
   return( str );
}


//--- Fin. -------------------------------------------------------------------
