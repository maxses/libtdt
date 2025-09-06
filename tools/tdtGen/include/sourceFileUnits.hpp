#if ! defined TDT_TDTGEN_SOURCE_FILE_UNITS_HPP
#define TDT_TDTGEN_SOURCE_FILE_UNITS_HPP
/**---------------------------------------------------------------------------
 *
 * @file    sourceFileUnits.hpp
 * @brief   Source file generator for unit header
 *
 * Print the object-to-string map.
 *
 * @date      20241003
 * @author    Maximilian Seesslen <mes@seesslen.net>
 * @copyright SPDX-License-Identifier: Apache-2.0
 *
 *--------------------------------------------------------------------------*/


/*--- Includes -------------------------------------------------------------*/


#include <QString>
#include <QFile>
#include <QDebug>
#include <object.hpp>
#include <sourceFile.hpp>


/*--- Declaration ----------------------------------------------------------*/


class CSourceFileUnits: public CSourceFile
{
      
   public:
      CSourceFileUnits( QString fileName )
         :CSourceFile( fileName )
      {
         start();
      };
      ~CSourceFileUnits()
      {
         close();
      }
      virtual void writeTop( ) override;
      virtual void writeBottom( ) override;
      void openEnum();
      void closeEnum();
};


/*--- Fin ------------------------------------------------------------------*/
#endif //   ? ! TDT_TDTGEN_SOURCE_FILE_UNITS_HPP
