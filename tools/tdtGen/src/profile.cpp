//----------------------------------------------------------------------------
///
///   \file    profile.cpp
///   \brief   Carries description for TDT-Profile
///
///            Print the enums
/// 
///   \author  Maximilian Seesslen <mes@seesslen.net>
///
//----------------------------------------------------------------------------


//--- Includes ---------------------------------------------------------------


#include <QJsonObject>
#include <QJsonArray>
#include <QDebug>
#include <profile.hpp>
#include <QDebug>


//--- Implementation----------------------------------------------------------


void CProfile::parseProfile( const QJsonObject& obj )
{
   if( obj.contains("base") )
   {
      setBase( obj["base"].toString().toInt( nullptr, 0) );
   }
   if( obj.contains("size") )
   {
      m_size=obj["size"].toString().toInt( nullptr, 0);
   }

   parseObjects( obj["objects"].toArray() );
   parseUnits( obj["units"].toArray() );
   parseCommands( obj["commands"].toArray() );
   parseLogs( obj["logs"].toArray() );
}


int CProfile::parseObjects( const QJsonArray& array )
{
   for( auto obj : array )
   {
      QJsonObject o=obj.toObject();
      QSharedPointer<CObject> object=QSharedPointer<CObject>(new 
         CObject( *this, o["offset"].toString().toInt( nullptr, 0 ),
                  o["name"].toString(),
                  o["description"].toString() ) );
      object->parse( o );
      qDebug() << "   Object: " << o["offset"].toString().toInt( nullptr, 0 );
      if( m_objects.contains( object->numericalValue() ) )
      {
         if( &m_objects[object->numericalValue()]->getProfile() != this )
         {
            qFatal("Object '%s' overlaps.", qPrintable( object->getName() ) );
         }
      }
      m_objects+=object;
   }
   return(0);
}


int CProfile::parseUnits( const QJsonArray& array )
{
   for( auto obj : array )
   {
      QJsonObject o=obj.toObject();
      QSharedPointer<CUnit> unit=QSharedPointer<CUnit>(new 
                                                               CUnit( *this, o["offset"].toString().toInt( nullptr, 0 ),
                                                                       o["name"].toString(),
                                                                       o["description"].toString() ) );
      unit->parse( o );
      qDebug() << "   Unit: " << o["offset"].toString().toInt( nullptr, 0 );
      m_units+=unit;
   }
   return(0);
}


int CProfile::parseCommands( const QJsonArray& array )
{
   for( auto obj : array )
   {
      QJsonObject o=obj.toObject();
      QSharedPointer<CCommand> command=QSharedPointer<CCommand>(new 
                                                         CCommand( *this, o["offset"].toString().toInt( nullptr, 0 ),
                                                               o["name"].toString(),
                                                               o["description"].toString() ) );
      command->parse( o );
      qDebug() << "   Command: " << o["offset"].toString().toInt( nullptr, 0 );
      if( m_commands.contains( command->numericalValue() ) )
      {
         if( &m_commands[ command->numericalValue()]->getProfile() != this )
         {
            qFatal("Cummand '%s' overlaps.", qPrintable( command->getName() ) );
         }
      }
      m_commands+=command;
   }
   return(0);
}


int CProfile::parseLogs( const QJsonArray& array )
{
   for( auto obj : array )
   {
      QJsonObject o=obj.toObject();
      QSharedPointer<CLog> log=QSharedPointer<CLog>(
               new CLog( *this, o["offset"].toString().toInt( nullptr, 0 ),
                        o["name"].toString(), o["description"].toString() ) );
      log->parse( o );
      qDebug() << "   Log: " << o["offset"].toString().toInt( nullptr, 0 );
      if( m_logs.contains( log->numericalValue() ) )
      {
         if( &m_logs[ log->numericalValue()]->getProfile() != this )
         {
            qFatal("Log '%s' overlaps.", qPrintable( log->getName() ) );
         }
      }
      m_logs+=log;
   }
   return(0);
}


const QString& CProfile::getName() const
{
   return(m_name);
}


int CProfile::generate()
{
   qDebug() << "      // Profile: " << m_name << "; " << m_desc;
   for( const auto& object : m_objects )
   {
      qDebug() << object->enumString();
      qDebug() << object->printerString();
   }
   return(0);
}


int CProfile::getBase() const
{
   return( m_base );
}


void CProfile::writeObjectsEnums( QTextStream& s )
{
   s << "\n";
   s << "      // Profile: " << m_name << "; " << m_desc << "\n";
   for( const auto& object : m_objects )
   {
      s << object->enumString() << "\n";
   }
   return;
}


void CProfile::writeUnitsEnums( QTextStream& s )
{
   s << "\n";
   s << "      // Profile: " << m_name << "; " << m_desc << "\n";
   for( const auto& unit : m_units )
   {
      s << unit->enumString() << "\n";
   }
   return;
}


void CProfile::writeCommandsEnums( QTextStream& s )
{
   s << "\n";
   s << "      // Profile: " << m_name << "; " << m_desc << "\n";
   for( const auto& command : m_commands )
   {
      s << command->enumString() << "\n";
   }
   return;
}


void CProfile::writeLogsEnums( QTextStream& s )
{
   s << "\n";
   s << "      // Profile: " << m_name << "; " << m_desc << "\n";
   for( const auto& log : m_logs )
   {
      s << log->enumString() << "\n";
   }
   return;
}


void CProfile::writeInfo( QTextStream& s )
{
   s << "\n";
   s << "Profile: " << m_name << "; " << m_desc << "\n";
   s.setIntegerBase(16);
   s << "   Base: 0x" << m_base;
   s << "   Size: 0x" << m_size;
   return;
}


void CProfile::writeObjectsPrinters( QTextStream& s )
{
   s << "\n";
   s << "      // Profile: " << m_name << "; " << m_desc << "\n";
   for( const auto& object : m_objects )
   {
      s << object->printerString() << "\n";
   }
   return;
}


void CProfile::writeUnitsPrinters( QTextStream& s )
{
   s << "\n";
   s << "      // Profile: " << m_name << "; " << m_desc << "\n";
   for( const auto& object : m_units )
   {
      s << object->printerString() << "\n";
   }
   return;
}


void CProfile::writeLogsPrinters( QTextStream& s )
{
   s << "\n";
   s << "      // Profile: " << m_name << "; " << m_desc << "\n";
   for( const auto& object : m_logs )
   {
      s << object->printerString() << "\n";
   }
   return;
}


//--- Fin. -------------------------------------------------------------------
