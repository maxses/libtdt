#if ! defined TDT_TDTGEN_PROFILE_HPP
#define TDT_TDTGEN_PROFILE_HPP
//----------------------------------------------------------------------------
///
///   \file    profile.hpp
///   \brief   Carries description for TDT-Profile
///
///            Print the enums
/// 
///   \author  Maximilian Seesslen <mes@seesslen.net>
///
//----------------------------------------------------------------------------


//--- Includes ---------------------------------------------------------------


#include <QString>
#include <QSharedPointer>
#include <QTextStream>
#include <QList>
#include <QTextStream>
#include <object.hpp>
#include <unit.hpp>
#include <command.hpp>
#include <logs.hpp>


//--- Implementation----------------------------------------------------------


class CProfile
{
   const QString m_name;
   const QString m_desc;
   int m_base=0;
   int m_size=0;
   QList <QSharedPointer<CObject>> m_objects;
   QList <QSharedPointer<CUnit>> m_units;
   QList <QSharedPointer<CCommand>> m_commands;
   QList <QSharedPointer<CCommand>> m_logs;
   
public:
   
   CProfile( const QString& name, const QString& desc )
       :m_name( name )
       ,m_desc( desc )
   {
      
   }
   void parseProfile( const QJsonObject& obj );
   int parseObjects( const QJsonArray& array );
   int parseUnits( const QJsonArray& array );
   int parseCommands( const QJsonArray& array );
   const QString& getName() const;
   int generate();
   void setBase( int base )
   {
      m_base=base;
   }
   int getBase() const;
   void writeObjectsEnums( QTextStream& s );
   void writeUnitsEnums( QTextStream& s );
   void writeCommandsEnums( QTextStream& s );
   void writeLogsEnums( QTextStream& s );
   void writeInfo( QTextStream& s );
   void writeObjectsPrinters( QTextStream& s );
   void writeUnitsPrinters( QTextStream& s );
   void writeLogsPrinters( QTextStream& s );
};


//--- Fin. -------------------------------------------------------------------
#endif //   ? ! TDT_TDTGEN_PROFILE_HPP
