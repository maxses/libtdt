#if ! defined TDT_TDTGEN_OBJECT_HPP
#define TDT_TDTGEN_OBJECT_HPP
/**---------------------------------------------------------------------------
 *
 * @file    object.hpp
 * @brief   Carries description for TDT-Object
 *
 * Print the enums.
 *
 * @date      20241003
 * @author    Maximilian Seesslen <mes@seesslen.net>
 * @copyright SPDX-License-Identifier: Apache-2.0
 *
 *--------------------------------------------------------------------------*/


/*--- Includes -------------------------------------------------------------*/


#include <QString>
#include <QJsonObject>
#include <QSharedPointer>


/*--- Declaration ----------------------------------------------------------*/


class CProfile;

class CObject
{
   const CProfile& m_profile;
   int m_object;
   const QString m_name;
   const QString m_desc;
   int m_size=0;
   bool m_profileInEnum=true;
   QString m_macro;
      
   public:
       
   CObject(CProfile& profile, int object, const QString& name, const QString& desc )
           :m_profile( profile )
           ,m_object( object )
           ,m_name( name )
           ,m_desc( desc )
       {
          
       };
   void parse( QJsonObject& o );
   QString enumString();
   void appendObjectNumber( QString& str, int objectNumber ) const;
   QString nameToEnum();
   QString printerString();
   void setSize( int size )
   {
      m_size=size;
   }
   virtual const QString nsPrefix() const
   {
      return( "Tdt::EObject::" );
   }
   void setProfileInEnum( bool doSo )
   {
      m_profileInEnum=doSo;
   }
   int numericalValue() const;
   bool operator ==(int nv) const
   {
      return( numericalValue() == nv );
   }
   const QString& getName()
   {
      return( m_name );
   }
   const CProfile& getProfile()
   {
      return( m_profile );
   }
   void setMacro(const QString& macro)
   {
      m_macro=macro;
   }
};


bool operator== (const QSharedPointer<CObject> c, const int nv);


/*--- Fin ------------------------------------------------------------------*/
#endif //   ? ! TDT_TDTGEN_OBJECT_HPP
