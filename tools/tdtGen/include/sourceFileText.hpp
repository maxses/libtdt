#if ! defined TDT_TDTGEN_SOURCE_FILE_TEXT_HPP
#define TDT_TDTGEN_SOURCE_FILE_TEXT_HPP
/**---------------------------------------------------------------------------
 *
 * @file    sourceFileText.hpp
 * @brief   Source file generator for generic text file
 *
 * Print som info text to a file.
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


class CSourceFileText: public CSourceFile
{
   public:
      CSourceFileText( QString fileName )
         :CSourceFile( fileName )
      {
         start();
      };
      ~CSourceFileText()
      {
         close();
      }
      virtual void writeTop( ) override;
      virtual void writeBottom( ) override;
};


/*--- Fin ------------------------------------------------------------------*/
#endif //   ? ! TDT_TDTGEN_SOURCE_FILE_TEXT_HPP
