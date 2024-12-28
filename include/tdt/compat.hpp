#ifndef TDT_COMPAT_HPP
#define TDT_COMPAT_HPP
//-----------------------------------------------------------------------------
//
// \brief   
//
//-----------------------------------------------------------------------------


#if defined USE_LEPTO
   #define qFatal    lFatal
   #define qDebug    lDebug
#else
   #define LDS(a, b) ( b )
   #define SEC_PER_MIN        ( 60 )
   #define SEC_PER_HOUR       ( SEC_PER_MIN * 60 )
   #define SEC_PER_DAY        ( SEC_PER_HOUR * 24 )
   
#endif


//---fin-----------------------------------------------------------------------
#endif // ? ! TDT_COMPAT_HPP
