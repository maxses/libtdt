#if ! defined TDT_TDTGEN_SOURCE_FILE_PRINTER_HPP
#define TDT_TDTGEN_SOURCE_FILE_PRINTER_HPP
/**---------------------------------------------------------------------------
 *
 * @file    sourceFilePrinter.hpp
 * @brief   Source file generator for printer header
 *
 * Print the object-to-string map
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


class CSourceFilePrinter: public CSourceFile
{
      
   public:
      CSourceFilePrinter( QString fileName )
         :CSourceFile( fileName )
      {
         start();
      };
      ~CSourceFilePrinter( )
      {
         close();
      }
      virtual void writeTop( ) override;
      void endObjects( );
      void startUnits( );
      void endUnits( );
      void startLogs( );
      void endLogs( );
      virtual void writeBottom( ) override;
};


/*--- Fin ------------------------------------------------------------------*/
#endif //   ? ! TDT_TDTGEN_SOURCE_FILE_PRINTER_HPP
