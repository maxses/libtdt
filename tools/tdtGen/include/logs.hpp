#if ! defined TDT_TDTGEN_LOG_HPP
#define TDT_TDTGEN_LOG_HPP
//----------------------------------------------------------------------------
///
///   \file    log.hpp
///   \brief   Carries description for TDT-node-log
///
///            Print the enums
/// 
///   \author  Maximilian Seesslen <mes@seesslen.net>
///
//----------------------------------------------------------------------------


//--- Includes ---------------------------------------------------------------


#include <QString>
#include <QJsonObject>
#include <object.hpp>


//--- Implementation----------------------------------------------------------


class CProfile;

class CLog: public CObject
{
   public:
       
   CLog(CProfile& profile, int log, const QString& name, const QString& desc )
      :CObject(profile, log, name, desc )
   {
      setProfileInEnum(false);
   };
   virtual const QString nsPrefix() const
   {
      return( "Tdt::ELog::" );
   }
};


//--- Fin. -------------------------------------------------------------------
#endif //   ? ! TDT_TDTGEN_LOG_HPP
