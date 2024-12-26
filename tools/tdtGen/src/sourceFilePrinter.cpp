//----------------------------------------------------------------------------
///
///   \file    sourceFilePrinter.cpp
///   \brief   Source file generator for object header
///
///            Print the object-to-string map
/// 
///   \author  Maximilian Seesslen <mes@seesslen.net>
///
//----------------------------------------------------------------------------


//--- Includes ---------------------------------------------------------------


#include <QString>
#include <QTextStream>
#include <sourceFilePrinter.hpp>


//--- Implementation----------------------------------------------------------


void CSourceFilePrinter::writeTop( )
{
   m_stream << "#if ! defined TDT_PRINTER_HPP\n";
   m_stream << "#define TDT_PRINTER_HPP\n";
   m_stream << "//---------------------------------------\n";
   m_stream << "// Generated file, do not change!\n";
   m_stream << "// tdtGen was used to create this file.\n";
   m_stream << "//---------------------------------------\n";
   m_stream << "\n";
   m_stream << "\n";
   m_stream << "namespace Tdt\n";
   m_stream << "{\n";
   m_stream << "\n";
   m_stream << "class CPrinterBase\n";
   m_stream << "{\n";
   m_stream << "   protected:";
   m_stream << "   const QMap<Tdt::EObject, const char *> m_objectMap\n";
   m_stream << "   {\n";
};


void CSourceFilePrinter::writeBottom( )
{
   m_stream << "   };\n";
   m_stream << "};\n";
   m_stream << "\n";
   m_stream << "} // namespace Tdt\n";
   m_stream << "\n";
   m_stream << "#endif // ! ? TDT_PRINTER_HPP\n";
};


//--- Fin. -------------------------------------------------------------------
