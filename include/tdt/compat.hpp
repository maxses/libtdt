#ifndef TDT_COMPAT_HPP
#define TDT_COMPAT_HPP
/**---------------------------------------------------------------------------
 *
 * @file    compat.hpp
 * @brief   Defines neccessary to let sources compile both for MCU and host
 *
 * For MCU target, liblepto and libbiwak are used. On host PC Qt libraries are 
 * used.
 * These macros for example use lepto debugging functions instead of Qt.
 *
 * @date      20241219
 * @author    Maximilian Seesslen <mes@seesslen.net>
 * @copyright SPDX-License-Identifier: Apache-2.0
 *
 *--------------------------------------------------------------------------*/


/*--- Defines --------------------------------------------------------------*/


#if defined USE_LEPTO
   // Lepto is used
   #define qFatal    lFatal
   #define qDebug    lDebug
#else
   // Qt is used; Leptos macros are missing
   #define LDS(a, b) ( b )
   #define SEC_PER_MIN        ( 60 )
   #define SEC_PER_HOUR       ( SEC_PER_MIN * 60 )
   #define SEC_PER_DAY        ( SEC_PER_HOUR * 24 )
   
#endif


/*--- Fin ------------------------------------------------------------------*/
#endif // ? ! TDT_COMPAT_HPP
