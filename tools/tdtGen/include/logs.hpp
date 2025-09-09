#if ! defined TDT_TDTGEN_LOG_HPP
#define TDT_TDTGEN_LOG_HPP
/**---------------------------------------------------------------------------
 *
 * @file    log.hpp
 * @brief   Carries description for TDT-node-log
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


/*--- Fin ------------------------------------------------------------------*/
#endif //   ? ! TDT_TDTGEN_LOG_HPP
