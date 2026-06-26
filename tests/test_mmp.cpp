/**---------------------------------------------------------------------------
 *
 * @file    test_mmp.cpp
 * @brief   Unit test for a MMP transfer
 *
 * Construct virtual pseudo units and start transfers on each. After some time
 * the transfer should have finished. The transfered data can be verified.
 *
 * Warning: Disfunctional at the moment.
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
#include <tdt/mmp.hpp>
#include <tdt/mmpNode.hpp>
#include <QElapsedTimer>
#include <QObject>
#include <QCoreApplication>
#include <QEventLoop>

#define TEST_ALL
#define STOP_ON_FAIL


/*--- Implementation -------------------------------------------------------*/


const char* data1{ R"(
Lorem ipsum dolor sit amet, consetetur sadipscing elitr, sed diam nonumy eirmod
tempor invidunt ut labore et dolore magna aliquyam erat, sed diam voluptua. At
vero eos et accusam et justo duo dolores et ea rebum. Stet clita kasd gubergren,
 no sea takimata sanctus est Lorem ipsum dolor sit amet. Lorem ipsum dolor sit
amet, consetetur sadipscing elitr, sed diam nonumy eirmod tempor invidunt ut
labore et dolore magna aliquyam erat, sed diam voluptua.)" };

const char* data2{ R"(
Non eram nescius, Brute, cum, quae summis ingeniis exquisitaque doctrina
philosophi Graeco sermone tractavissent, ea Latinis litteris mandaremus, fore
ut hic noster labor in varias reprehensiones incurreret. nam quibusdam, et iis
quidem non admodum indoctis, totum hoc displicet philosophari. quidam autem non
tam id reprehendunt, si remissius agatur, sed tantum studium tamque multam
operam ponendam in eo non arbitrantur.)" };


void hexDump(const char* data, int len)
{
   int pos=0;
   while(pos<len)
   {
      printf("0x%04X | ", pos);
      for(int i1=0; i1<0x10; i1++)
      {
         printf("0x%02X ", data[pos+i1]);
      }
      printf(" | ");
      for(int i1=0; i1<0x10; i1++)
      {
         char c=data[pos+i1];
         printf("%c", ( ( c>=' ' ) && ( c<='~' ) ) ? c : '.' );
      }
      pos+=0x10;
      printf("\n");
   }
}


TEST_CASE( "MMP simple", "[default]" )
{
   SECTION( "construct" )
   {
      Tdt::CMmpNode dut1(8);
      Tdt::CMmpNode dut2(9);
      int argc=1;
      char* argv[1]{ (char*)"Main" };
      QCoreApplication core(argc, argv);
      const char* data="Hello World";
      QElapsedTimer elapsed;
      QEventLoop loop;
      bool finished=false;

      /* Must not connect the signal sendTdtMessage() to slot receiveTdtMessage()
       * directly. Sending an ACK would directly end up in the receive function
       * before having the old shred finished.
       */
      
      QObject::connect( &dut1, SIGNAL( signalSendTdtMessage( const Tdt::CMessage& ) ),
                       &dut2, SLOT( pushRxMessage( const Tdt::CMessage& ) ) );
      
      QObject::connect( &dut2, SIGNAL( signalSendTdtMessage( const Tdt::CMessage& ) ),
                       &dut1, SLOT( pushRxMessage( const Tdt::CMessage& ) ) );
      
      QObject::connect( &dut1, SIGNAL( signalHandleMmpTransfer( const Tdt::CMmpTransfer& ) ),
                       &dut1, SLOT( dummyHandleMmpTransfer( const Tdt::CMmpTransfer& ) ) );
      
      QObject::connect( &dut2, SIGNAL( signalHandleMmpTransfer( const Tdt::CMmpTransfer& ) ),
                       &dut2, SLOT( dummyHandleMmpTransfer( const Tdt::CMmpTransfer & ) ) );
      
      for(int i1=0; i1<10; i1++)
      {
         dut1.m_tx.setData( Tdt::EMmpCommand::writeApplicationFlash, data, strlen( data ) );
         dut1.m_tx.setCounterNodeId( dut2.getNodeId() );
         dut1.startTx();
         
         finished=false;
         elapsed.start();
         
         while( elapsed.elapsed( ) < 30 * 1000 )
         {
            dut1.testEventLoop();
            dut2.testEventLoop();
            
            if( ( dut1.state() == Tdt::ENodeState::idle )
                &&  ( dut2.state() == Tdt::ENodeState::idle ) )
            {
               printf("Both channels finished\n");
               finished=true;
               break;
            }
            loop.processEvents();
         }
         
         REQUIRE( finished == true );
         
         REQUIRE( dut2.getTotalRxTransfers() == i1 + 1 );
         //REQUIRE( dut1.getTotalRxTransfers() == 1 );
         
         dut1.m_tx.dump();
         dut2.m_tx.dump();
         
         REQUIRE ( memcmp( dut2.m_rx.data(), data, strlen(data) ) == 0 );
      }
   }
}


TEST_CASE( "MMP full duplex", "[default]" )
{
   SECTION( "construct" )
   {
      Tdt::CMmpNode dut1(8);
      Tdt::CMmpNode dut2(9);
      QElapsedTimer elapsed;
      int argc=1;
      char* argv[1]{ (char*)"Main" };
      QCoreApplication core(argc, argv);

      /* Must not connect the signal sendTdtMessage() to slot receiveTdtMessage()
       * directly. Sending an ACK would directly end up in the receive function
       * before having the old shred finished.
       */

      QObject::connect( &dut1, SIGNAL( signalSendTdtMessage( const Tdt::CMessage& ) ),
               &dut2, SLOT( pushRxMessage( const Tdt::CMessage& ) ) );

      QObject::connect( &dut2, SIGNAL( signalSendTdtMessage( const Tdt::CMessage& ) ),
               &dut1, SLOT( pushRxMessage( const Tdt::CMessage& ) ) );

      QObject::connect( &dut1, SIGNAL( signalHandleMmpTransfer( const Tdt::CMmpTransfer& ) ),
               &dut1, SLOT( dummyHandleMmpTransfer( const Tdt::CMmpTransfer& ) ) );

      QObject::connect( &dut2, SIGNAL( signalHandleMmpTransfer( const Tdt::CMmpTransfer& ) ),
               &dut2, SLOT( dummyHandleMmpTransfer( const Tdt::CMmpTransfer& ) ) );
      
      for(int i1=0; i1<1; i1++)
      {
         elapsed.start();
         
         dut1.m_rx.wipe();
         dut2.m_rx.wipe();

         dut1.m_tx.setData( Tdt::EMmpCommand::writeApplicationFlash, data1, strlen(data1) );
         dut1.m_tx.setCounterNodeId( dut2.getNodeId() );
         dut1.startTx();
   
         #if 1
            dut2.m_tx.setData( Tdt::EMmpCommand::writeApplicationFlash, data2, strlen(data2) );
            dut2.m_tx.setCounterNodeId( dut1.getNodeId() );
            dut2.startTx();
         #endif
         
         QEventLoop loop;
         bool finished=false;
   
         while( elapsed.elapsed( ) < 30 * 1000 )
         {
            dut1.testEventLoop();
            dut2.testEventLoop();
   
            if( ( dut1.state() == Tdt::ENodeState::idle )
                &&  ( dut2.state() == Tdt::ENodeState::idle ) )
            {
               printf("Both channels finished\n");
               finished=true;
               break;
            }
            // Would block: loop.exec();
            loop.processEvents();
         }
   
         // Flush receive buffers
         //for(int i1=0; i1<1; i1++)
         {
            dut1.testEventLoop();
            dut2.testEventLoop();
            loop.processEvents();
         }
         
         REQUIRE( finished == true );
         
         REQUIRE( dut2.getTotalRxTransfers() == i1 + 1 );
         REQUIRE( dut1.getTotalRxTransfers() == i1 + 1 );
   
         dut1.m_tx.dump();
         dut2.m_tx.dump();
         
         REQUIRE( dut2.state() == Tdt::ENodeState::idle );
         REQUIRE( dut1.state() == Tdt::ENodeState::idle );

         REQUIRE( (int)dut2.m_tx.returnCode() == (int)dut1.getNodeId() );
         REQUIRE( (int)dut1.m_tx.returnCode() == (int)dut2.getNodeId() );

         // Timed out and there is still data
         //printf( "Data left DUT1: %d (org: %d)\n", dut1.m_tx.dataLeft(), (int)strlen(data1) );
         //printf( "Data left DUT2: %d (org: %d)\n", dut2.m_tx.dataLeft(), (int)strlen(data2) );
         
         // The counters may be reset, shred-position is zero. isFinished will
         // be false
         //REQUIRE ( dut1.m_tx.isFinished() );
         //REQUIRE ( dut2.m_tx.isFinished() );
   
         //hexDump( dut2.m_rx.data(), 0x200 );
   
         // received data is the vis-à-vis
         REQUIRE ( memcmp( dut1.m_rx.data(), data2, strlen(data2) ) == 0 );
         REQUIRE ( memcmp( dut2.m_rx.data(), data1, strlen(data1) ) == 0 );
      }
   }
}


/*--- Fin ------------------------------------------------------------------*/
