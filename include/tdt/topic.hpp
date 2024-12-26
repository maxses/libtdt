#ifndef TDT_TOPIC_HPP
#define TDT_TOPIC_HPP
//-----------------------------------------------------------------------------
//
//
//
//-----------------------------------------------------------------------------


#include <tdt/message.hpp>
#include <QTextStream>
#include <QMap>


#define ENUM_MAP( a, e ) { a::e, "" #e }
#define ENUM_MAP_TEXT( a, e, t ) { a::e, t }

#define TDT_TOPIC( topic ) \
constexpr EObject operator +( EObject ## topic object) \
{ \
   return( (EObject)object ); \
};


#define OBJECT_OFFSET_NODE          0x400
#define OBJECT_OFFSET_ENVIRONMENT   0x400
#define OBJECT_OFFSET_PLANT         0x400


class CTopic
{
   public:
      CTopic( const char* name, const QMap<Tdt::EObject, const char *>&& _objectMap );
      void init();
};


//---fin-----------------------------------------------------------------------
#endif // ? ! TDT_TOPIC_HPP
