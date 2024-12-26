//----------------------------------------------------------------------------
///
///   \file    sourceFileObjects.cpp
///   \brief   Source file generator for object header
///
///            Print the enums
/// 
///   \author  Maximilian Seesslen <mes@seesslen.net>
///
//----------------------------------------------------------------------------


//--- Includes ---------------------------------------------------------------


#include <QString>
#include <QTextStream>
#include <sourceFileObjects.hpp>


//--- Implementation----------------------------------------------------------


void CSourceFileObjects::writeTop( )
{
   m_stream << "#if ! defined TDT_OBJECTS_HPP\n";
   m_stream << "#define TDT_OBJECTS_HPP\n";
   m_stream << "//---------------------------------------\n";
   m_stream << "// Generated file, do not change!\n";
   m_stream << "// tdtGen was used to create this file.\n";
   m_stream << "//---------------------------------------\n";
   m_stream << "\n";
   m_stream << "\n";
   m_stream << "namespace Tdt\n";
   m_stream << "{\n";
   m_stream << "\n";
   m_stream << "enum class EObject: uint16_t\n";
   m_stream << "{\n";
};


void CSourceFileObjects::writeBottom( )
{
   m_stream << "};\n";
   m_stream << "\n";
   m_stream << "} // namespace Tdt\n";
   m_stream << "\n";
   m_stream << "#endif // ! ? TDT_OBJECTS_HPP\n";
};


//--- Fin. -------------------------------------------------------------------
