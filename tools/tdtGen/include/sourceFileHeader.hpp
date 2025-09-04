#if ! defined TDT_TDTGEN_SOURCE_FILE_HEADER_HPP
#define TDT_TDTGEN_SOURCE_FILE_HEADER_HPP
/**---------------------------------------------------------------------------
 *
 * @file    sourceFileHeader.hpp
 * @brief   Source file generator for generic header
 *
 * Print an generic header.
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


class CSourceFileHeader: public CSourceFile
{
   QString m_name;
   QString m_enum;
   
   public:
      CSourceFileHeader( QString fileName, QString name, QString _enum, QStringList includes )
         :CSourceFile( fileName )
         ,m_name( name )
         ,m_enum( _enum )
      {
         m_includes += includes;
         start();
      };
      ~CSourceFileHeader()
      {
         close();
      }
      virtual void writeTop( ) override;
      virtual void writeBottom( ) override;
};


/*--- Fin ------------------------------------------------------------------*/
#endif //   ? ! TDT_TDTGEN_SOURCE_FILE_HEADER_HPP
