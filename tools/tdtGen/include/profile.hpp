#if ! defined TDT_TDTGEN_PROFILE_HPP
#define TDT_TDTGEN_PROFILE_HPP
/**---------------------------------------------------------------------------
 *
 * @file    profile.hpp
 * @brief   Carries description for TDT-Profile
 *
 * Print the enums.
 *
 * @date      20241003
 * @author    Maximilian Seesslen <mes@seesslen.net>
 * @copyright SPDX-License-Identifier: Apache-2.0
 *
 *--------------------------------------------------------------------------*/


/*--- Includes -------------------------------------------------------------*/


#include <QString>
#include <QSharedPointer>
#include <QTextStream>
#include <QList>
#include <QTextStream>
#include <object.hpp>
#include <unit.hpp>
#include <command.hpp>
#include <logs.hpp>


/*--- Declaration ----------------------------------------------------------*/


class CProfile
{
   const QString m_name;
   const QString m_desc;
   int m_base=0;
   int m_unitsBase=0;
   int m_size=0;
   QList <QSharedPointer<CObject>> m_objects;
   QList <QSharedPointer<CUnit>> m_units;
   QList <QSharedPointer<CCommand>> m_commands;
   QList <QSharedPointer<CLog>> m_logs;
   QStringList m_includes;
   
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
   int parseLogs( const QJsonArray& array );
   const QString& getName() const;
   int generate();
   void setBase( int base )
   {
      m_base=base;
   }
   void setUnitsBase( int base )
   {
      m_unitsBase=base;
   }
   int getBase() const;
   int getUnitsBase() const;
   void writeObjectsEnums( QTextStream& s );
   void writeUnitsEnums( QTextStream& s );
   void writeUnitsEnumTypeEnums( QTextStream& s );
   void writeCommandsEnums( QTextStream& s );
   void writeLogsEnums( QTextStream& s );
   void writeInfo( QTextStream& s );
   void writeObjectsPrinters( QTextStream& s );
   void writeUnitsPrinters( QTextStream& s );
   void writeLogsPrinters( QTextStream& s );
   const QStringList& getIncludes() const
   {
      return(m_includes);
   }
};


/*--- Fin ------------------------------------------------------------------*/
#endif //   ? ! TDT_TDTGEN_PROFILE_HPP
