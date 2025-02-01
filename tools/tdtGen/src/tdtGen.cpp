//----------------------------------------------------------------------------
///
///   \file    sourceFileUnits.cpp
///   \brief   Source file generator for unit header
///
///            Print the enums
/// 
///   \author  Maximilian Seesslen <mes@seesslen.net>
///
//----------------------------------------------------------------------------


//--- Includes ---------------------------------------------------------------


#include <QString>
#include <QTextStream>
#include <tdtGen.hpp>
#include <sourceFileObjects.hpp>
#include <sourceFilePrinter.hpp>
#include <sourceFileUnits.hpp>
#include <sourceFileText.hpp>
#include <sourceFileHeader.hpp>


//--- Implementation----------------------------------------------------------


int CTdtGen::generate()
{
   if( m_profiles.count() == 0 )
   {
      qFatal("No profiles");
   }
   
   CSourceFileObjects sfo( m_outDir + "/objects.hpp");
   QTextStream& so=sfo.getStream();
   
   for( const auto& profile : m_profiles )
   {
      profile->writeObjectsEnums( so );
   }
   
   CSourceFilePrinter sfp( m_outDir + "/printerBase.hpp" );
   QTextStream& sp=sfp.getStream();
   
   for( const auto& profile : m_profiles )
   {
      profile->writeObjectsPrinters( sp );
   }
   sfp.endObjects();
   
   sfp.startUnits();
   for( const auto& profile : m_profiles )
   {
      profile->writeUnitsPrinters( sp );
   }
   sfp.endUnits();
   
   sfp.startLogs();
   for( const auto& profile : m_profiles )
   {
      profile->writeLogsPrinters( sp );
   }
   sfp.endLogs();
   
   CSourceFileUnits sfu( m_outDir + "/units.hpp" );
   QTextStream& su=sfu.getStream();
   
   for( const auto& profile : m_profiles )
   {
      profile->writeUnitsEnums( su );
   }
   
   CSourceFileHeader sfc( m_outDir + "/commands.hpp", "commands", "ECommand" );
   QTextStream& sc=sfc.getStream();
   
   for( const auto& profile : m_profiles )
   {
      profile->writeCommandsEnums( sc );
   }
   
   CSourceFileHeader sfl( m_outDir + "/logs.hpp", "logs", "ELog" );
   QTextStream& sl=sfl.getStream();
   
   for( const auto& profile : m_profiles )
   {
      profile->writeLogsEnums( sl );
   }
   
   CSourceFileText sft( m_outDir + "/info.txt" );
   QTextStream& st=sft.getStream();
   
   for( const auto& profile : m_profiles )
   {
      profile->writeInfo( st );
   }
   
   return(0);
}


//--- Fin. -------------------------------------------------------------------
