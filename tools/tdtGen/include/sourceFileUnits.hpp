#if ! defined TDT_TDTGEN_SOURCE_FILE_UNITS_HPP
#define TDT_TDTGEN_SOURCE_FILE_UNITS_HPP
//----------------------------------------------------------------------------
///
///   \file    sourceFileUnits.hpp
///   \brief   Source file generator for unit header
///
///            Print the object-to-string map
/// 
///   \author  Maximilian Seesslen <mes@seesslen.net>
///
//----------------------------------------------------------------------------


//--- Includes ---------------------------------------------------------------


#include <QString>
#include <QFile>
#include <QDebug>
#include <object.hpp>
#include <sourceFile.hpp>


//--- Implementation----------------------------------------------------------


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
};


//--- Fin. -------------------------------------------------------------------
#endif //   ? ! TDT_TDTGEN_SOURCE_FILE_UNITS_HPP
