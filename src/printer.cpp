//-----------------------------------------------------------------------------
//
//
//
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------


#include <tdt/printer.hpp>


//-----------------------------------------------------------------------------


namespace Tdt
{

EObject operator+(const EObject &a, int value)
{
   return( (EObject)( (int)a + value ) );
};

int operator-(const EObject &a, EObject b)
{
   return( (int)a - (int)b );
};


} // namespace Tdt

//QMap<Tdt::EObject, const char *> CCanTdtPrinter::m_objectMap;


//---fin-----------------------------------------------------------------------
