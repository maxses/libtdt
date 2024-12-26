#if ! defined TDT_TDTGEN_SOURCE_FILE_HEADER_HPP
#define TDT_TDTGEN_SOURCE_FILE_HEADER_HPP
//----------------------------------------------------------------------------
///
///   \file    sourceFileHeader.hpp
///   \brief   Source file generator for generic header
///
///            Print an generic header
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


class CSourceFileHeader: public CSourceFile
{
   QString m_name;
   QString m_enum;
   
   public:
      CSourceFileHeader( QString fileName, QString name, QString _enum )
         :CSourceFile( fileName )
         ,m_name( name )
         ,m_enum( _enum )
      {
         start();
      };
      ~CSourceFileHeader()
      {
         close();
      }
      virtual void writeTop( ) override;
      virtual void writeBottom( ) override;
};


//--- Fin. -------------------------------------------------------------------
#endif //   ? ! TDT_TDTGEN_SOURCE_FILE_HEADER_HPP
