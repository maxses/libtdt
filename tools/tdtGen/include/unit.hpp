#if ! defined TDT_TDTGEN_UNIT_HPP
#define TDT_TDTGEN_UNIT_HPP
//----------------------------------------------------------------------------
///
///   \file    unit.hpp
///   \brief   Carries description for TDT-Unit
///
///            Print the enums
/// 
///   \author  Maximilian Seesslen <mes@seesslen.net>
///
//----------------------------------------------------------------------------


//--- Includes ---------------------------------------------------------------


#include <QString>
#include <QJsonObject>


//--- Implementation----------------------------------------------------------


class CProfile;

class CUnit
{
   const CProfile& m_profile;
   int m_unit;
   const QString m_name;
   const QString m_desc;
   QString m_postfix;
   int m_decimalPower=1;
   QString m_format;
      
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
   void appendObjectNumber( QString& str, int objectNumber ) const;
   QString nameToEnum();
   QString printerString();
   const QString nsPrefix() const
   {
      return( "Tdt::EUnit::" );
   }
};


//--- Fin. -------------------------------------------------------------------
#endif //   ? ! TDT_TDTGEN_UNIT_HPP
