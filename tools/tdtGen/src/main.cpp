/**---------------------------------------------------------------------------
 *
 * @file    main.cpp
 * @brief   Main file
 *
 * Instanciate CTdtGen.
 *
 * @date      20241003
 * @author    Maximilian Seesslen <mes@seesslen.net>
 * @copyright SPDX-License-Identifier: Apache-2.0
 *
 *--------------------------------------------------------------------------*/


/*--- Includes -------------------------------------------------------------*/


#include <QCoreApplication>
#include <QCommandLineParser>
#include <tdtGen.hpp>


/*--- Implementation -------------------------------------------------------*/


int main( int argc, char* argv[] )
{
   QCoreApplication qapp(argc, argv);
   QCoreApplication::setApplicationName("TDTGenen");
   //QCoreApplication::setApplicationVersion( GIT_TAG );

   QCommandLineParser parser;
   parser.setApplicationDescription("TDT source file generator");
   parser.addHelpOption();
   parser.addVersionOption();

   QCommandLineOption oInput(QStringList() << "i" << "input",
                                  "Input files", "files");
   QCommandLineOption oOutput(QStringList() << "o" << "outDir",
                             "Output directory", "directory");

   parser.addOptions( { oInput, oOutput } );
   parser.process(qapp);
   
   if(  parser.value( oInput ).isEmpty() || ( !parser.isSet( oOutput ) ) )
   {
      parser.showHelp(22);
   }
   
   CTdtGen tdtGen( parser.value( oOutput ) );
   tdtGen.addSources( parser.values( oInput ) );
   tdtGen.doit();

   return(0);
}


/*--- Fin ------------------------------------------------------------------*/
