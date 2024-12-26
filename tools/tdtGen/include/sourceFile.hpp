#if ! defined TDT_TDTGEN_SOURCE_FILE_HPP
#define TDT_TDTGEN_SOURCE_FILE_HPP
//----------------------------------------------------------------------------
///
///   \file    sourceFile.hpp
///   \brief   Source file generator
///
///            Print stuff
/// 
///   \author  Maximilian Seesslen <mes@seesslen.net>
///
//----------------------------------------------------------------------------


//--- Includes ---------------------------------------------------------------


#include <QString>
#include <QFile>
#include <object.hpp>


//--- Implementation----------------------------------------------------------


class CSourceFile
{
      QString m_fileName;
      QFile m_file;
      
   protected:
      QTextStream m_stream;
      
   public:
      CSourceFile( QString fileName )
         :m_fileName( fileName )
         ,m_file( fileName )
         , m_stream( &m_file )
      {
         if( !m_file.open( QIODevice::WriteOnly | QIODevice::Text ) )
         {
            qFatal("Could not open output file");
         }
      }
      void start()
      {
         writeTop( );
      };
      virtual ~CSourceFile( )
      {
      }
      void close()
      {
         writeBottom( );
         m_file.close( );
      }
      virtual void writeTop( )=0;
      virtual void writeBottom( )=0;
      QTextStream& getStream()
      {
         return( m_stream );
      }
};


//--- Fin. -------------------------------------------------------------------
#endif //   ? ! TDT_TDTGEN_SOURCE_FILE_HPP
