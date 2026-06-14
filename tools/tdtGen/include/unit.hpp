#if ! defined TDT_TDTGEN_UNIT_HPP
#define TDT_TDTGEN_UNIT_HPP
/**---------------------------------------------------------------------------
 *
 * @file    unit.hpp
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


#include <QString>
#include <QJsonObject>


/*--- Declaration ----------------------------------------------------------*/


class CProfile;

class CUnit
{
   const CProfile& m_profile;
   int m_unit;
   const QString m_name;
   const QString m_desc;
   QString m_prefix;
   QString m_postfix;
   int m_divider=1;
   QString m_format;
   QMap< int, QString > m_values;
      
   public:
       
   CUnit(CProfile& profile, int unit, const QString& name, const QString& desc )
           :m_profile( profile )
           ,m_unit( unit )
           ,m_name( name )
           ,m_desc( desc )
   {
      return;
   };
   void parse( QJsonObject& o );
   QString enumString();
   void writeEnumTypeEnums( QTextStream& s );
   void appendUnitNumber( QString& str, int unitNumber ) const;
   QString nameToEnum();
   static QString nameToEnum( const QString& name );
   QString printerString();
   const QString nsPrefix() const
   {
      return( "Tdt::EUnit::" );
   }
};


/*--- Fin ------------------------------------------------------------------*/
#endif //   ? ! TDT_TDTGEN_UNIT_HPP
