#ifndef TDT_DB_HPP
#define TDT_DB_HPP
//-----------------------------------------------------------------------------
//
//
//
//-----------------------------------------------------------------------------


#include <QMap>


namespace Tdt
{


class CDb
{
   private:
      QMap<int, QString> m_nodeNames;
      QMap<int, QString> m_nodeObjectNames;
      
      /// \brief Combine Node-Id with e.g. Object number to have a single map
      constexpr int combinedId( int id, int subId ) const
      {
         return ( ( id << 16 ) | subId );
      }

   public:
      
      CDb( );
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
      QString getObjectName( int nodeId, int object ) const
      {
         if( m_nodeObjectNames.contains( combinedId( nodeId, object ) ) )
         {
            return( m_nodeObjectNames[ combinedId( nodeId, object )  ] );
         }
         //return( QString("%1 [0x%2]").arg( nodeId ).arg( object, 1, 16 ) );
         return( QString{} );
      }
};


} // namespace Tdt;


//---fin-----------------------------------------------------------------------
#endif // ? ! TDT_DB_HPP
