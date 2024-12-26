#if ! defined TDT_TDTGEN_SOURCE_FILE_OBJECTS_HPP
#define TDT_TDTGEN_SOURCE_FILE_OBJECTS_HPP
//----------------------------------------------------------------------------
///
///   \file    sourceFileObjects.hpp
///   \brief   Source file generator for object header
///
///            Print the enums
/// 
///   \author  Maximilian Seesslen <mes@seesslen.net>
///
//----------------------------------------------------------------------------


//--- Includes ---------------------------------------------------------------


#include <QString>
#include <QFile>
#include <object.hpp>
#include <sourceFile.hpp>


//--- Implementation----------------------------------------------------------


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


//--- Fin. -------------------------------------------------------------------
#endif //   ? ! TDT_TDTGEN_SOURCE_FILE_OBJECTS_HPP
