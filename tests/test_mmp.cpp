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

#include <tdt/message.hpp>
#include <tdt/mmp.hpp>
#include <QElapsedTimer>
#include <QObject>


//--- Own ----------------------------


#define TEST_ALL
#define STOP_ON_FAIL


//---Implementation------------------------------------------------------------


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


void handleQueues(Tdt::CMmpNode& n1, Tdt::CMmpNode& n2)
{
   /*
   while( n1.m_rx.m_txList.count() )
   {
      n2.receive( n1.m_rx.m_txList.last() );
      n1.m_rx.m_txList.removeLast();
   }
   while( n2.m_tx.m_txList.count() )
   {
      n1.receive( n2.m_tx.m_txList.last() );
      n2.m_tx.m_txList.removeLast();
   }
   */
};


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
         printf("%C", ( ( c>=' ' ) && ( c<='~' ) ) ? c : '.' );
      }
      pos+=0x10;
      printf("\n");
   }
}


TEST_CASE( "MMP full duplex", "[default]" )
{
   SECTION( "construct" )
   {
      Tdt::CMmpNode dut1;
      Tdt::CMmpNode dut2;
      QElapsedTimer elapsed;
      
      elapsed.start();
      
      dut1.m_tx.m_data.setData( Tdt::EMmpObject::writeApplicationFlash, data1, strlen(data1) );
      dut1.m_tx.startTx();
      dut2.m_tx.m_data.setData( Tdt::EMmpObject::writeApplicationFlash, data2, strlen(data2) );
      dut2.m_tx.startTx();
      
      while( elapsed.elapsed( ) < 1 * 1000 )
      {
         handleQueues( dut1, dut2 );
         handleQueues( dut2, dut1 );
         //if( dut1.finishedRx() && dut2.finishedRx() )
         {
            break;
         }
      }
      
      //REQUIRE ( dut1.finishedRx() );
      //REQUIRE ( dut2.finishedRx() );
      
      //hexDump( dut1.m_rx.m_data.data(), 0x20 );
      
      // received data is the vis-à-vis
      REQUIRE ( memcmp( dut1.m_rx.m_data.data(), data2, strlen(data2) ) == 0 );
      REQUIRE ( memcmp( dut2.m_rx.m_data.data(), data1, strlen(data1) ) == 0 );
   }
}


//---fin-----------------------------------------------------------------------
