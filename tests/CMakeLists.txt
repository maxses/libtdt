#------------------------------------------------------------------------------
#
# \brief	CMakeLists.txt file for "libbiwak"
#
#------------------------------------------------------------------------------


cmake_minimum_required(VERSION 3.11)
project( tdt_tests )

find_package(Catch2 3 QUIET)
if (Catch2_FOUND)
    add_definitions( "-DCATCH_V3" )
    set(CATCH2_TARGET Catch2::Catch2WithMain)
else()
    find_package(Catch2 2 REQUIRED)
    add_definitions( "-DCATCH_V2" )
    set(CATCH2_TARGET Catch2::Catch2)
endif()

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
      ${CATCH2_TARGET}
)


add_test(
   NAME tdt_tests
   COMMAND tdt_tests
   WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
)


#------------------------------------------------------------------------------
