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

#if defined ( CATCH_V3 )
   #include <catch2/catch_test_macros.hpp>
#elif defined ( CATCH_V2 )
   #include <catch2/catch.hpp>
#elif defined ( CATCH_V1 )
   #include <catch/catch.hpp>
#else
   #error "Either 'catch' or 'catch2' has to be installed"
#endif

#include <lepto/can_message_tdt.h>

//--- Own ----------------------------

#define TEST_ALL
#define STOP_ON_FAIL

//---Implementation------------------------------------------------------------


TEST_CASE( "CAN TDT", "[default]" )
{
   SECTION( "construct" )
   {
      const unsigned char data[]=
      {
         0x55, 0x00,
         (char)Tdt::EUnit::centiCelsius ,
         0x00,    // reserved
         // 2200 = 0x00000898
         0x98, 0x08,
         0x00, 0x00,
      };

      CCanMessage msg(
         ( (int)Tdt::EFunctionCode::sendObject
                  << Tdt::FUNCTIONCODE_BITSHIFT )
               | 0x10
               , 8
               , data );
      Tdt::CMessage tdt(msg);
      msg.setLen(0x10000);

      REQUIRE ( tdt.getLen() == 8 );
      REQUIRE ( (int)tdt.getTdtObject() == 0x55 );
      REQUIRE ( (int)tdt.getId() == 0x410 );
      REQUIRE ( (int)tdt.getTdtValueInt() == 2200 );

      Tdt::CMessage tdt2(0x7f, (Tdt::EFunctionCode)0xff
                          , (Tdt::EObject)0xFFFF
                          , (Tdt::EUnit)0xff
                          , Tdt::SValue{._uint=0xFFFFFFFF} );
      tdt2=tdt;

      REQUIRE ( tdt2.getLen() == 8 );
      REQUIRE ( (int)tdt2.getTdtObject() == 0x55 );
      REQUIRE ( (int)tdt2.getId() == 0x410 );
      REQUIRE ( (int)tdt2.getTdtValueInt() == 2200 );
      
      Tdt::CMessage sendMessage
          {0x32,
           Tdt::EFunctionCode::sendObject,
                                Tdt::EObject::logCode, Tdt::EUnit::logCode, Tdt::SValue{.logCode=ELogCategory::Critical | 1}};
      REQUIRE ( sendMessage.getLen() == 8 );
      
   }
}


//---fin-----------------------------------------------------------------------
