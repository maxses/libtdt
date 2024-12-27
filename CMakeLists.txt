#-----------------------------------------------------------------------------
#
# \brief	CMakeLists.txt file for "libtdt"
#
#-----------------------------------------------------------------------------


cmake_minimum_required(VERSION 3.11)
project( tdt LANGUAGES CXX )


# if ( NOT MCU_REV OR "${MCU_REV}" MATCHES "^x86" )
if( "${CMAKE_SYSTEM_PROCESSOR}" STREQUAL "x86_64" )
   set(CMAKE_AUTOUIC ON)
   set(CMAKE_AUTOMOC ON)
   set(CMAKE_AUTORCC ON)
   set( QT_COMPONENTS Core Widgets Network Xml )

   # Check either Qt6 or Qt5
   find_package( QT NAMES Qt5 Qt6 COMPONENTS ${QT_COMPONENTS} REQUIRED )
   find_package( Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS ${QT_COMPONENTS} )

   add_definitions( "-fPIC" )
   set( MCU_PLATFORM "linux" )
endif( )


#--- tdtGen-------------------------------------------------------------------


set( TDT_OUTPUT_DIR
   ${CMAKE_BINARY_DIR}/tdt_generated
)

set ( 
    TDT_GEN_HEADER
        ${TDT_OUTPUT_DIR}/tdt/gen/objects.hpp
        ${TDT_OUTPUT_DIR}/tdt/gen/printerBase.hpp
)

list( APPEND
    tdtProfiles
    ${CMAKE_CURRENT_SOURCE_DIR}/tdt/node.json
    ${CMAKE_CURRENT_SOURCE_DIR}/tdt/environment.json
    ${CMAKE_CURRENT_SOURCE_DIR}/tdt/power.json
    ${CMAKE_CURRENT_SOURCE_DIR}/tdt/mmp.json
    ${CMAKE_CURRENT_SOURCE_DIR}/tdt/log.json
    ${CMAKE_CURRENT_SOURCE_DIR}/tdt/plants.json
    ${CMAKE_CURRENT_SOURCE_DIR}/tdt/generic.json
    ${CMAKE_CURRENT_SOURCE_DIR}/tdt/recorder.json
    ${CMAKE_CURRENT_SOURCE_DIR}/tdt/global.json
    ${CMAKE_CURRENT_SOURCE_DIR}/tdt/infoDisplay.json
    ${CMAKE_CURRENT_SOURCE_DIR}/tdt/facility.json
)

include(ExternalProject)

ExternalProject_Add (
   tdtGen
      SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/tools/tdtGen
      CMAKE_ARGS  
         -DCMAKE_MODULE_PATH='${CMAKE_MODULE_PATH_JOINED2}'
         -DCMAKE_INSTALL_PREFIX=${CMAKE_BINARY_DIR}
      INSTALL_DIR ${CMAKE_BINARY_DIR}
)

# have to transform it somehow; ";" as seperator did not work well
STRING (JOIN ":" tdtProfilesJoined ${tdtProfiles} )

file( MAKE_DIRECTORY ${TDT_OUTPUT_DIR}/tdt/gen )

add_custom_target( tdtGenerate
    COMMAND
        SOURCE_DIR=${CMAKE_SOURCE_DIR} 
        OUTPUT_DIR=${CMAKE_BINARY_DIR}/git_version 
        PROJECT=${CMAKE_PROJECT_NAME} 
        ${CMAKE_BINARY_DIR}/tools/bin/tdtGen -i ${tdtProfilesJoined}
               -o ${TDT_OUTPUT_DIR}/tdt/gen
    DEPENDS tdtGen ${tdtProfiles}
)

add_custom_command(
    OUTPUT
         ${TDT_GEN_HEADER}
    DEPENDS
         tdtGen tdtGenerate
)


#--- libtdt ------------------------------------------------------------------


set( headers
   include/tdt/message.hpp
   include/tdt/mmp.hpp
   ${TDT_GEN_HEADER}
)

set( sources
   src/message.cpp
   src/mmpTransfer.cpp
   src/mmpNode.cpp
   
   ${tdtProfiles}
)

if ( NOT MCU_REV OR "${MCU_REV}" MATCHES "x86" )
   set(sources
      ${sources}
      # src/ansiparser.cpp
      src/printer.cpp
      src/db.cpp
	)
   set(headers
      ${headers}
      include/tdt/printer.hpp
      include/tdt/db.hpp
   )
else()
   set(sources
		${sources}
   )
   set(headers
       ${headers}
   )
endif()

add_library(
   ${PROJECT_NAME}
   STATIC
      ${sources}
      ${headers}
      ${tdt_HEADERS_MOC}
)

if ( COMMON_CONFIG_HEADER )
   set ( TDT_CONFIG_HEADER "${COMMON_CONFIG_HEADER}" )
elseif( EXISTS "${CMAKE_SYSROOT}/usr/include/lepto/config.h" )
   set ( TDT_CONFIG_HEADER "${CMAKE_SYSROOT}/usr/include/lepto/config.h" )
endif()

# If you dont want a config header, stop lepto to complain about this.
if( TDT_CONFIGURED )
   target_compile_definitions(
      ${PROJECT_NAME}
      PUBLIC
         "-DTDT_CONFIGURED"
   )
endif()

if( TDT_CONFIG_HEADER )
   get_filename_component( TDT_CONFIG_PATH ${TDT_CONFIG_HEADER} DIRECTORY )
   get_filename_component( TDT_CONFIG_FILE ${TDT_CONFIG_HEADER} NAME )

   target_compile_options(
      ${PROJECT_NAME}
      PRIVATE
         "SHELL:-include ${TDT_CONFIG_HEADER}"
      PUBLIC
         "SHELL:-include ${TDT_CONFIG_HEADER}"
   )

   set_source_files_properties(
      ${sources}
      ${headers}
      PROPERTIES
      OBJECT_DEPENDS
      ${TDT_CONFIG_HEADER}
   )

   set( TDT_CONFIGURED Yes )
endif()

if( NOT TDT_CONFIGURED )
   message( ERROR "TDT_CONFIGURED not set." )
   message( FATAL_ERROR "No config header for tdt defined. Either set "
            "'COMMON_CONFIG_HEADER' or 'TDT_CONFIG_HEADER' in your projects "
            "root cmake file. If you are sure that you don't need it, set "
            "'TDT_CONFIGURED' cmake variable")
endif()

if( USE_LEPTO )
   target_link_libraries(
      ${PROJECT_NAME}
      lepto
   )
endif( USE_LEPTO )

target_include_directories( ${PROJECT_NAME}
   PRIVATE
      ${CMAKE_BINARY_DIR}/tdt_generated
   INTERFACE
      ${CMAKE_BINARY_DIR}/tdt_generated
)

if( "${MCU_PLATFORM}" MATCHES "linux" )
   target_link_libraries(
		${PROJECT_NAME}
      Qt${QT_VERSION_MAJOR}::Core
      Qt${QT_VERSION_MAJOR}::Xml
	)
endif()

target_include_directories(
   ${PROJECT_NAME}
   PRIVATE
      include/
   INTERFACE
      include/
)

if( "${CMAKE_SYSTEM_PROCESSOR}" STREQUAL "x86_64" )
    add_subdirectory(tests)
endif()


#-----------------------------------------------------------------------------
