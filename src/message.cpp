/**---------------------------------------------------------------------------
 *
 * @file    message.cpp
 * @brief   The tdt message structures and functions
 *
 * @date      20241219
 * @author    Maximilian Seesslen <src@seesslen.net>
 * @copyright SPDX-License-Identifier: Apache-2.0
 *
 *--------------------------------------------------------------------------*/


/*--- Includes -------------------------------------------------------------*/


#include <tdt/message.hpp>


/*--- Implementation -------------------------------------------------------*/


namespace Tdt
{


EObject operator+(const EObject &a, int value)
{
   return( (EObject)( (int)a + value ) );
};


} // namespace Tdt


//---fin-----------------------------------------------------------------------
