/**---------------------------------------------------------------------------
 *
 * @file    printer.hpp
 * @brief   Generate text fragments from tdt messages
 *
 * Those text strings can be used by user interfaces on host PC.
 * For text fragments to be used on target microcontroller, see "printer_biwak.hpp".
 * 
 * @date      20241214
 * @author    Maximilian Seesslen <mes@seesslen.net>
 * @copyright SPDX-License-Identifier: Apache-2.0
 *
 *--------------------------------------------------------------------------*/


/*--- Includes -------------------------------------------------------------*/


#include <tdt/printer.hpp>


/*--- Implementation -------------------------------------------------------*/


namespace Tdt
{


EObject operator+(const EObject &a, int value)
{
   return( (EObject)( (int)a + value ) );
};


}; // namespace Tdt


/*--- Fin ------------------------------------------------------------------*/
