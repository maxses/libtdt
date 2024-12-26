//----------------------------------------------------------------------------
///
///   \file    sourceFileUnits.cpp
///   \brief   Source file generator for unit header
///
///            Print the enums
/// 
///   \author  Maximilian Seesslen <mes@seesslen.net>
///
//----------------------------------------------------------------------------


//--- Includes ---------------------------------------------------------------


#include <QString>
#include <QTextStream>
#include <sourceFileUnits.hpp>


//--- Implementation----------------------------------------------------------


void CSourceFileUnits::writeTop( )
{
   m_stream << "#if ! defined TDT_GEN_UNITS_HPP\n";
   m_stream << "#define TDT_GEN_UNITS_HPP\n";
   m_stream << "//---------------------------------------\n";
   m_stream << "// Generated file, do not change!\n";
   m_stream << "// tdtGen was used to create this file.\n";
   m_stream << "//---------------------------------------\n";
   m_stream << "\n";
   m_stream << "\n";
   m_stream << "namespace Tdt\n";
   m_stream << "{\n";
   m_stream << "\n";
   m_stream << "enum class EUnit: uint8_t\n";
   m_stream << "{\n";
};


void CSourceFileUnits::writeBottom( )
{
   m_stream << "};\n";
   m_stream << "\n";
   m_stream << "} // namespace Tdt\n";
   m_stream << "\n";
   m_stream << "#endif // ! ? TDT_GEN_UNITS_HPP\n";
};


//--- Fin. -------------------------------------------------------------------
