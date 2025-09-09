/**---------------------------------------------------------------------------
 *
 * @file    test_message.cpp
 * @brief   Basic unit tests for CAN message structs
 *
 * Use constructors and verify getter functions.
 *
 * @date      20220619
 * @author    Maximilian Seesslen <mes@seesslen.net>
 * @copyright SPDX-License-Identifier: Apache-2.0
 *
 *--------------------------------------------------------------------------*/


/*--- Includes -------------------------------------------------------------*/


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
#include <tdt/printer.hpp>
#include <QDebug>


#define TEST_ALL
#define STOP_ON_FAIL


/*--- Implementation -------------------------------------------------------*/


TEST_CASE( "TDT Printer", "[default]" )
{
   SECTION( "enum" )
   {
      Tdt::CPrinter printer;
      
      {
         Tdt::CMessage tdt1(0x7f, (Tdt::EFunctionCode)Tdt::EFunctionCode::sendObject
                            , Tdt::EObject::facilityRoom
                            , Tdt::EUnit::room
                            , Tdt::SValue{ .anyEnum= toAny( Tdt::ERoom::kitchen ) } );
         qWarning() << "Value: " << printer.printValue( tdt1 );
         REQUIRE( printer.printValue( tdt1 ) == "Kitchen" );
      }
      
      {
         Tdt::CMessage tdt1(0x7f, (Tdt::EFunctionCode)Tdt::EFunctionCode::sendObject
                            , Tdt::EObject::logCode
                            , Tdt::EUnit::centiCelsius
                            , Tdt::SValue{ ._int= 2387 } );
         qWarning() << "Value: " << printer.printValue( tdt1 );
         REQUIRE( printer.printValue( tdt1 ) == "23.87 °C" );
      }
      
      {
         Tdt::CMessage tdt1(0x7f, (Tdt::EFunctionCode)Tdt::EFunctionCode::sendObject
                            , Tdt::EObject::logCode
                            , Tdt::EUnit::capacity
                            , Tdt::SValue{ ._int= 16384 } );
         qWarning() << "Value: " << printer.printValue( tdt1 );
         REQUIRE( printer.printValue( tdt1 ) == "16 KB" );
      }
      
      {
         Tdt::CMessage tdt1(0x7f, (Tdt::EFunctionCode)Tdt::EFunctionCode::sendObject
                            , Tdt::EObject::logCode
                            , Tdt::EUnit::systemState
                            , Tdt::SValue{ ._int= (int)Tdt::ESystemState::reverseBootloader } );
         qWarning() << "Value: " << printer.printValue( tdt1 );
         REQUIRE( printer.printValue( tdt1 ) == "Reverse Bootloader" );
      }
      
      {
         Tdt::CMessage tdt1(0x7f, (Tdt::EFunctionCode)Tdt::EFunctionCode::sendObject
                            , Tdt::EObject::nodeFirmwareVersion
                            , Tdt::EUnit::version
                            , Tdt::SValue{ .softwareVersion {
                                    .drift { .dirty=1, .bootloader=1, .distance=12 },
                                    .patch=30, .minor=20, .major=10
                                    }
                              }
         );
         qWarning() << "Value: " << printer.printValue( tdt1 );
         REQUIRE( printer.printValue( tdt1 ) == "BL v10.20.30-12+" );
      }
      
      {
         Tdt::CMessage tdt1(0x7f, (Tdt::EFunctionCode)Tdt::EFunctionCode::sendObject
                            , Tdt::EObject::nodeFirmwareVersion
                            , Tdt::EUnit::version
                            , Tdt::SValue{ .softwareVersion {
                                    .drift { .dirty=0, .bootloader=0, .distance=0 },
                                    .patch=113, .minor=20, .major=10
                                }
                            }
                            );
         qWarning() << "Value: " << printer.printValue( tdt1 );
         REQUIRE( printer.printValue( tdt1 ) == "v10.20.113-0" );
      }
      
      {
         Tdt::CMessage tdt1(0x7f, (Tdt::EFunctionCode)Tdt::EFunctionCode::sendObject
                            , Tdt::EObject::nodeFirmwareVersion
                            , Tdt::EUnit::index
                            , Tdt::SValue{ ._int = 0x30 }
                            );
         qWarning() << "Value: " << printer.printValue( tdt1 );
         REQUIRE( printer.printValue( tdt1 ) == "[48]" );
      }
      
      {
         Tdt::CMessage tdt1(0x7f, (Tdt::EFunctionCode)Tdt::EFunctionCode::sendObject
                            , Tdt::EObject::nodeFirmwareVersion
                            , Tdt::EUnit::article
                            , Tdt::SValue{ ._int = (int)Tdt::EArticle::mainSwitch }
                            );
         qWarning() << "Value: " << printer.printValue( tdt1 );
         REQUIRE( printer.printValue( tdt1 ) == "MainSwitch" );
      }
   }
}


/*--- Fin ------------------------------------------------------------------*/
