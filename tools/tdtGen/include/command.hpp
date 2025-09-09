#if ! defined TDT_TDTGEN_COMMAND_HPP
#define TDT_TDTGEN_COMMAND_HPP
/**---------------------------------------------------------------------------
 *
 * @file    command.hpp
 * @brief   Carries description for TDT-node-Command
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
#include <object.hpp>


/*--- Declaration ----------------------------------------------------------*/


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


/*--- Fin ------------------------------------------------------------------*/
#endif //   ? ! TDT_TDTGEN_COMMAND_HPP
