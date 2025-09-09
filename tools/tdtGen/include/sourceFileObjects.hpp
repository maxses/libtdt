#if ! defined TDT_TDTGEN_SOURCE_FILE_OBJECTS_HPP
#define TDT_TDTGEN_SOURCE_FILE_OBJECTS_HPP
/**---------------------------------------------------------------------------
 *
 * @file    sourceFileObjects.hpp
 * @brief   Source file generator for object header
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
#include <QFile>
#include <object.hpp>
#include <sourceFile.hpp>


/*--- Declaration ----------------------------------------------------------*/


class CSourceFileObjects: public CSourceFile
{
   public:
      CSourceFileObjects( QString fileName )
         :CSourceFile( fileName )
      {
         start();
      };
      ~CSourceFileObjects( )
      {
         close();
      }
      virtual void writeTop( ) override;
      virtual void writeBottom( ) override;
};


/*--- Fin ------------------------------------------------------------------*/
#endif //   ? ! TDT_TDTGEN_SOURCE_FILE_OBJECTS_HPP
