#if ! defined TDT_TDTGEN_SOURCE_FILE_TEXT_HPP
#define TDT_TDTGEN_SOURCE_FILE_TEXT_HPP
//----------------------------------------------------------------------------
///
///   \file    sourceFileText.hpp
///   \brief   Source file generator for generic text file
///
///            Print som info text to a file
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


//--- Fin. -------------------------------------------------------------------
#endif //   ? ! TDT_TDTGEN_SOURCE_FILE_TEXT_HPP
