/**---------------------------------------------------------------------------
 *
 * @file    tdtGen.hpp
 * @brief   Main tdtGen class
 *
 * Generates all the files.
 *
 * @date      20241003
 * @author    Maximilian Seesslen <mes@seesslen.net>
 * @copyright SPDX-License-Identifier: Apache-2.0
 *
 *--------------------------------------------------------------------------*/


/*--- Includes -------------------------------------------------------------*/


#include <QSharedPointer>
#include <QDebug>
#include <QFile>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <profile.hpp>


/*--- Declaration ----------------------------------------------------------*/


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
      
      int generate();

      int parseFile( const QString &fileName )
      {
         QFile file( fileName );
         int profiles=0;
         
         if( ! file.open( QIODevice::ReadOnly | QIODevice::Text ) )
         {
            qFatal("Could not oen file '%s'", qPrintable( fileName ) );
         }
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


/*--- Fin ------------------------------------------------------------------*/
