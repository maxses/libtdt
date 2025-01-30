//----------------------------------------------------------------------------
///
/// \file   main.cpp
///
/// \brief  main file for pinmux
///
/// \date   20220619
/// \author Maximilian Seesslen <mes@seesslen.net>
///
//----------------------------------------------------------------------------


//---Documentation------------------------------------------------------------


//---Includes -----------------------------------------------------------------


//---General--------------------------

#define CATCH_CONFIG_MAIN     1

#if defined ( CATCH_V3 )
   #include <catch2/catch_test_macros.hpp>
#elif defined ( CATCH_V2 )
   #include <catch2/catch.hpp>
#elif defined ( CATCH_V1 )
   #include <catch/catch.hpp>
#else
   #error "Either 'catch' or 'catch2' has to be installed"
#endif

#include <tdt/message.hpp>

//--- Own ----------------------------

#define TEST_ALL
#define STOP_ON_FAIL

//---Implementation------------------------------------------------------------


//---fin-----------------------------------------------------------------------
