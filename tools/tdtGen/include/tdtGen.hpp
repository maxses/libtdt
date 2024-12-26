//----------------------------------------------------------------------------
///
///   \file    tdtGen.hpp
///   \brief   Carries description for TDT-Objectobject
///
///            Print the enums
/// 
///   \author  Maximilian Seesslen <mes@seesslen.net>
///
//----------------------------------------------------------------------------


//--- Includes ---------------------------------------------------------------


#include <QSharedPointer>
#include <QDebug>
#include <QFile>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <profile.hpp>
#include <sourceFileObjects.hpp>
#include <sourceFilePrinter.hpp>
#include <sourceFileUnits.hpp>


//--- Declaration ------------------------------------------------------------


class CTdtGen
{
   private:
      QString m_outDir;
      QList<QString> m_inputs;
      QList<QSharedPointer<CProfile>> m_profiles;
       
   public:
      
      CTdtGen( QString outDir )
           :m_outDir(outDir)
      {};
       
      void addSources( QList<QString> inputs )
      {
         for( const auto& s : inputs )
         {
            m_inputs+=s.split(":");
         }
         qDebug() << "Inputs: " << m_inputs;
      }
      
      int doit()
      {
         parse();
         generate();
         return(0);
      }
      
      int parse()
      {
         int profiles=0;
         for(auto file : m_inputs)
         {
            profiles+=parseFile(file);
         }
         qDebug( "Found %d profiles", profiles );
         return(profiles);
      }
      
      int generate()
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
         
         CSourceFileUnits sfu( m_outDir + "/units.hpp" );
         QTextStream& su=sfu.getStream();
         
         for( const auto& profile : m_profiles )
         {
            profile->writeUnitsEnums( su );
         }
         
         return(0);
      }
      
      int parseFile( const QString &fileName )
      {
         QFile file( fileName );
         int profiles=0;
         
         file.open( QIODevice::ReadOnly | QIODevice::Text );
         QByteArray ba=file.readAll();
         QJsonParseError error;
         QJsonDocument doc=QJsonDocument::fromJson( ba, &error );
         QJsonObject obj=doc.object();
         QJsonArray jsonArray = obj["profiles"].toArray();
         for(auto node : jsonArray)
         {
            parseProfile( node.toObject() );
         }
         if( error.error != QJsonParseError::NoError )
         {
            qCritical("Parse error");
            qFatal("fin.");
         }
         else
         {
            qDebug("Parsing ok");
         }
         profiles+=jsonArray.count();
         
         return(profiles);
      }
      
      int parseProfile( const QJsonObject& obj )
      {
         qDebug() << "Profile: " << obj["name"].toString();
         QSharedPointer<CProfile> profile=QSharedPointer<CProfile>(
            new CProfile( obj["name"].toString(), obj["description"].toString() ) );
         
         //profile->parseObjects( obj["objects"].toArray() );
         profile->parseProfile( obj );
         m_profiles+=profile;
         
         return(0);
      }
};
