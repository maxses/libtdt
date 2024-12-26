//-----------------------------------------------------------------------------
//
//
//
//-----------------------------------------------------------------------------


#include <tdt/topic.hpp>
#include <tdt/printer.hpp>
#include <QTextStream>
#include <QMap>


CTopic::CTopic( const char* name, const QMap<Tdt::EObject, const char *>&& _objectMap )
{
   //CCanTdtPrinter::m_objectMap.insert( _objectMap );
   /*
   if( ! strcmp( name, "Any") )
   {
      return;
   }
   qFatal("Added %s", name);
   */
}


void CTopic::init( )
{
   
}


//---fin-----------------------------------------------------------------------
