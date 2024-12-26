#if ! defined TDT_TDTGEN_SOURCE_FILE_PRINTER_HPP
#define TDT_TDTGEN_SOURCE_FILE_PRINTER_HPP
//----------------------------------------------------------------------------
///
///   \file    sourceFilePrinter.hpp
///   \brief   Source file generator for printer header
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


//--- Implementation----------------------------------------------------------


class CSourceFilePrinter
{
      QString m_fileName;
      QFile m_file;
      QTextStream m_stream;
      
   public:
      CSourceFilePrinter( QString fileName )
         :m_fileName( fileName )
         ,m_file( fileName )
         , m_stream( &m_file )
      {
         if( !m_file.open( QIODevice::WriteOnly | QIODevice::Text ) )
         {
            qFatal("Could not open output file");
         }
         qCritical() << "###" << fileName;
         writeTop( );
      };
      ~CSourceFilePrinter()
      {
         writeBottom( );
         m_file.close();
      };
      void writeTop( );
      void writeBottom( );
      QTextStream& getStream()
      {
         return( m_stream );
      }
};


//--- Fin. -------------------------------------------------------------------
#endif //   ? ! TDT_TDTGEN_SOURCE_FILE_PRINTER_HPP
