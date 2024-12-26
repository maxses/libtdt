//----------------------------------------------------------------------------
///
///   \file    profile.cpp
///   \brief   Carries description for TDT-Profile
///
///            Print the enums
/// 
///   \author  Maximilian Seesslen <mes@seesslen.net>
///
//----------------------------------------------------------------------------


//--- Includes ---------------------------------------------------------------


#include <QJsonObject>
#include <QJsonArray>
#include <profile.hpp>


//--- Implementation----------------------------------------------------------


void CProfile::parseProfile( const QJsonObject& obj )
{
   if( obj.contains("base") )
   {
      setBase( obj["base"].toString().toInt( nullptr, 0) );
   }

   parseObjects( obj["objects"].toArray() );
}


int CProfile::parseObjects( const QJsonArray& array )
{
   for( auto obj : array )
   {
      QJsonObject o=obj.toObject();
      QSharedPointer<CObject> object=QSharedPointer<CObject>(new 
         CObject( *this, o["offset"].toString().toInt( nullptr, 0 ),
                  o["name"].toString(),
                  o["description"].toString() ) );
      if( o.contains("size") )
      {
         object->setSize( o["size"].toString().toInt(nullptr, 0) );
      }
      qDebug() << "   Object: " << obj.toObject()["offset"].toString().toInt( nullptr, 0 );
      m_objects+=object;
   }
   return(0);
}


const QString& CProfile::getName() const
{
   return(m_name);
}


int CProfile::generate()
{
   qDebug() << "      // Profile: " << m_name << "; " << m_desc;
   for( const auto& object : m_objects )
   {
      qDebug() << object->enumString();
      qDebug() << object->printerString();
   }
   return(0);
}


int CProfile::getBase() const
{
   return( m_base );
}


void CProfile::writeEnums( QTextStream& s )
{
   s << "\n";
   s << "      // Profile: " << m_name << "; " << m_desc << "\n";
   for( const auto& object : m_objects )
   {
      s << object->enumString() << "\n";
   }
   return;
}


void CProfile::writePrinters( QTextStream& s )
{
   s << "\n";
   s << "      // Profile: " << m_name << "; " << m_desc << "\n";
   for( const auto& object : m_objects )
   {
      s << object->printerString() << "\n";
   }
   return;
}


//--- Fin. -------------------------------------------------------------------
