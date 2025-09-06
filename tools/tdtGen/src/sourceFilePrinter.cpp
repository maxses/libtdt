/**---------------------------------------------------------------------------
 *
 * @file    sourceFilePrinter.cpp
 * @brief   Source file generator for object header
 *
 * Print the object-to-string map
 *
 * @date      20241003
 * @author    Maximilian Seesslen <mes@seesslen.net>
 * @copyright SPDX-License-Identifier: Apache-2.0
 *
 *--------------------------------------------------------------------------*/


/*--- Includes -------------------------------------------------------------*/


#include <QString>
#include <QTextStream>
#include <sourceFilePrinter.hpp>


/*--- Implementation -------------------------------------------------------*/


void CSourceFilePrinter::writeTop( )
{
   m_stream << "#if ! defined TDT_PRINTER_BASE_HPP\n";
   m_stream << "#define TDT_PRINTER_BASE_HPP\n";
   m_stream << "//---------------------------------------\n";
   m_stream << "// Generated file, do not change!\n";
   m_stream << "// tdtGen was used to create this file.\n";
   m_stream << "//---------------------------------------\n";
   m_stream << "\n";
   m_stream << "\n";
   m_stream << "#include <QMap>\n";
   m_stream << "#include <tdt/gen/objects.hpp>\n";
   m_stream << "#include <tdt/gen/units.hpp>\n";
   m_stream << "#include <tdt/gen/logs.hpp>\n";
   m_stream << "\n";
   m_stream << "\n";
   m_stream << "namespace Tdt\n";
   m_stream << "{\n";
   m_stream << "\n";
   m_stream << "class CPrinterBase\n";
   m_stream << "{\n";
   m_stream << "   protected:\n";
   m_stream << "   const QMap<Tdt::EObject, const char *> m_objectMap\n";
   m_stream << "   {\n";
};


void CSourceFilePrinter::endObjects( )
{
   m_stream << "   };\n";
   m_stream << "\n";
};

void CSourceFilePrinter::startUnits( )
{
   m_stream << "   struct SUnitDesc {\n";
   m_stream << "      const char* name;\n";
   m_stream << "      const char* prefix;\n";
   m_stream << "      const char* postfix;\n";
   m_stream << "      char format;\n";
   m_stream << "      int divider;\n";
   m_stream << "      QMap<int, QString> enumValues;\n";
   m_stream << "   };\n";
   m_stream << "\n";
   m_stream << "   const QMap<Tdt::EUnit, SUnitDesc> m_unitMap\n";
   m_stream << "   {\n";
};


void CSourceFilePrinter::endUnits( )
{
   m_stream << "   };\n";
   m_stream << "\n";
};


void CSourceFilePrinter::startLogs( )
{
   m_stream << "   const QMap<Tdt::ELog, const char*> m_logMap\n";
   m_stream << "   {\n";
};


void CSourceFilePrinter::endLogs( )
{
   m_stream << "   };\n";
   m_stream << "\n";
};


void CSourceFilePrinter::writeBottom( )
{
   m_stream << "};\n";
   m_stream << "\n";
   m_stream << "} // namespace Tdt\n";
   m_stream << "\n";
   m_stream << "#endif // ! ? TDT_PRINTER_BASE_HPP\n";
};


/*--- Fin ------------------------------------------------------------------*/
