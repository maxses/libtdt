#if ! defined TDT_TDTGEN_OBJECT_HPP
#define TDT_TDTGEN_OBJECT_HPP
//----------------------------------------------------------------------------
///
///   \file    object.hpp
///   \brief   Carries description for TDT-Object
///
///            Print the enums
/// 
///   \author  Maximilian Seesslen <mes@seesslen.net>
///
//----------------------------------------------------------------------------


//--- Includes ---------------------------------------------------------------


#include <QString>


//--- Implementation----------------------------------------------------------


class CProfile;

class CObject
{
   const CProfile& m_profile;
   int m_object;
   const QString m_name;
   const QString m_desc;
   int m_size=0;
      
   public:
       
   CObject(CProfile& profile, int object, const QString& name, const QString& desc )
           :m_profile( profile )
           ,m_object( object )
           ,m_name( name )
           ,m_desc( desc )
       {
          
       };
   QString enumString();
   void appendObjectNumber( QString& str, int objectNumber ) const;
   QString nameToEnum();
   QString printerString();
   void setSize( int size )
   {
      m_size=size;
   }
   const QString nsPrefix() const
   {
      return( "Tdt::EObject::" );
   }
};


//--- Fin. -------------------------------------------------------------------
#endif //   ? ! TDT_TDTGEN_OBJECT_HPP
