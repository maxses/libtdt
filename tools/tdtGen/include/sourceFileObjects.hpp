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


//--- Implementation----------------------------------------------------------


class CSourceFileObjects
{
      QString m_fileName;
      QFile m_file;
      QTextStream m_stream;
      
   public:
      CSourceFileObjects( QString fileName )
         :m_fileName( fileName )
         ,m_file( fileName )
         , m_stream( &m_file )
      {
         if( !m_file.open( QIODevice::WriteOnly | QIODevice::Text ) )
         {
            qFatal("Could not open output file");
         }
         writeTop( );
      };
      ~CSourceFileObjects( )
      {
         writeBottom( );
         m_file.close( );
      }
      void writeTop( );
      void writeBottom( );
      QTextStream& getStream()
      {
         return( m_stream );
      }
};


//--- Fin. -------------------------------------------------------------------
#endif //   ? ! TDT_TDTGEN_SOURCE_FILE_OBJECTS_HPP
