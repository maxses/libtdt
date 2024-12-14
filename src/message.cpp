//-----------------------------------------------------------------------------
//
//
//
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------


#include <lepto/can_message_tdt.h>


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

#if 0
constexpr bool matchesSubIndexedObject( Tdt::EObject object, Tdt::EObject base )
{
   return( ( (uint32_t)object & MASK_SUBINDEX_OBJECT ) == (uint32_t)base );
}
#endif

} // namespace Tdt


//---fin-----------------------------------------------------------------------
