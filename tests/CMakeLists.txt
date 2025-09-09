#------------------------------------------------------------------------------
#
# \brief	CMakeLists.txt file for "libbiwak"
#
#------------------------------------------------------------------------------


cmake_minimum_required(VERSION 3.11)
project( tdt_tests )

find_package( Catch2 3 )
if( NOT Catch2_FOUND )
   find_package( Catch )
   if( NOT Catch_FOUND )
      # find_package does not work for Catch, at least on ubuntu 20.04
      if( EXISTS /usr/include/catch )
         add_definitions( "-DCATCH_V1" )
      elseif( EXISTS /usr/include/catch2 )
         if( EXISTS /usr/include/catch2/catch_test_macros.hpp )
            add_definitions( "-DCATCH_V3" )
         else()
            add_definitions( "-DCATCH_V2" )
         endif()
      else( EXISTS /usr/include/catch2 )
         message( FATAL_ERROR "Install eeither Catch or Catch2")
      endif( EXISTS /usr/include/catch )
   endif( NOT Catch_FOUND )
   add_definitions( "-DCATCH_V1" )
else( NOT Catch2_FOUND)
   add_definitions( "-DCATCH_V3" )
   set( CATCH_LIBRARY "Catch2::Catch2WithMain" )
endif( NOT Catch2_FOUND )

set(
   headers
)

set(sources
   test_main.cpp
   test_message.cpp
   test_mmp.cpp
   test_printer.cpp
)

add_executable(
   tdt_tests
   ${sources}
   ${headers}
)


target_link_libraries(
   tdt_tests
   PRIVATE
      tdt
      ${CATCH_LIBRARY}
)


add_test(
   NAME tdt_tests
   COMMAND tdt_tests
   WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
)


#------------------------------------------------------------------------------
