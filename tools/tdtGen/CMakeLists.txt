#---------------------------------------------------------------------
#
# \brief CMake file for tdtGen
#
#---------------------------------------------------------------------

cmake_minimum_required(VERSION 3.16)
PROJECT( tdtGen CXX C)

find_package( QT NAMES Qt5 Qt6 COMPONENTS Core REQUIRED )
find_package( Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Core )

add_executable(
    ${PROJECT_NAME}
    src/main.cpp
    src/tdtGen.cpp
    src/object.cpp
    src/profile.cpp
    src/unit.cpp
    src/sourceFileObjects.cpp
    src/sourceFilePrinter.cpp
    src/sourceFileUnits.cpp
    src/sourceFileText.cpp
    src/sourceFileHeader.cpp
    include/tdtGen.hpp
    include/object.hpp
    include/profile.hpp
    include/unit.hpp
    include/command.hpp
    include/sourceFileObjects.hpp
    include/sourceFilePrinter.hpp
    include/sourceFileUnits.hpp
    include/sourceFileText.hpp
    include/sourceFileHeader.hpp
)

target_link_libraries(
   ${PROJECT_NAME}
   Qt${QT_VERSION_MAJOR}::Core
)

target_include_directories(
   ${PROJECT_NAME}
   PRIVATE
      ${CMAKE_CURRENT_SOURCE_DIR}/include
)

install (
    TARGETS ${PROJECT_NAME}
    RUNTIME DESTINATION tools/bin
)


#---fin---------------------------------------------------------------

