#ifndef TDT_PRESET_POST_PROCESS_H
#define TDT_PRESET_POST_PROCESS_H
//----------------------------------------------------------------------------
///
/// \brief  Config preset post processingn
///
///         Enable features that are needed in the unit tests.
///
/// \date   20260705
/// \author Maximilian Seesslen <src@seesslen.net>
///
//----------------------------------------------------------------------------


#if ! defined ( STM32 )

   #define CONFIG_TDT_PEDANTIC                        1

#endif


//---fin-----------------------------------------------------------------------
#endif // ? ! TDT_PRESET_POST_PROCESS_H
