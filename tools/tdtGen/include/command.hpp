#if ! defined TDT_TDTGEN_COMMAND_HPP
#define TDT_TDTGEN_COMMAND_HPP
//----------------------------------------------------------------------------
///
///   \file    command.hpp
///   \brief   Carries description for TDT-node-Command
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

class CCommand: public CObject
{
   public:
       
   CCommand(CProfile& profile, int object, const QString& name, const QString& desc )
      :CObject(profile, object, name, desc )
   {
      setProfileInEnum(false);
   };
   virtual const QString nsPrefix() const
   {
      return( "Tdt::ECommand::" );
   }
};


//--- Fin. -------------------------------------------------------------------
#endif //   ? ! TDT_TDTGEN_COMMAND_HPP
