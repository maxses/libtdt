#ifndef LEPTO_CAN_TDT_DB_HPP
#define LEPTO_CAN_TDT_DB_HPP
//-----------------------------------------------------------------------------
//
//
//
//-----------------------------------------------------------------------------


#if defined USE_LEPTO
   //#include <lepto/can_message.h>
   #include <lepto/log.h>
#endif

#include <stdio.h>               // snprintf
#include <tdt/message.hpp>
#include <QTextStream>
#include <QMap>

#define ENUM_MAP( a, e ) { a::e, "" #e }
#define EID( id, subId ) ( ( id << 16 ) | subId )

namespace Tdt
{

class CCanTdtDb
{
   private:
      QMap<int, QString> m_nodeNames;
      QMap<int, QString> m_nodeSubIdNames;
      QMap<int, QString> m_nodeObjectNames;

   public:
      
      CCanTdtDb( );
       bool hasNodeId( int nodeId )
      {
          return( m_nodeNames.contains( nodeId ) );
      }
      QString getNodeName(int nodeId) const
		{
			if( m_nodeNames.contains( nodeId ) )
			{
				return( m_nodeNames[ nodeId ] );
			}
         return( QString("#%1").arg( nodeId ) );
		}
      QString getSubIdName( int nodeId, int subNodeId ) const
      {
         if( m_nodeSubIdNames.contains( EID( nodeId, subNodeId ) ) )
         {
            return( m_nodeSubIdNames[ EID( nodeId, subNodeId )  ] );
         }
         return( QString("?%1[%2]").arg( nodeId ).arg( subNodeId ) );
      }
      QString getObjectName( int nodeId, int object ) const
      {
         if( m_nodeObjectNames.contains( EID( nodeId, object ) ) )
         {
            return( m_nodeObjectNames[ EID( nodeId, object )  ] );
         }
         //return( QString("%1 [0x%2]").arg( nodeId ).arg( object, 1, 16 ) );
         return( QString{} );
      }
};

} // namespace Tdt;

//---fin-----------------------------------------------------------------------
#endif // ? ! LEPTO_CAN_TDT_DB_HPP
