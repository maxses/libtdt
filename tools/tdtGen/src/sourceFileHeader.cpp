/**---------------------------------------------------------------------------
 *
 * @file    sourceFileHeader.cpp
 * @brief   Source file generator for generic header
 *
 * Print generic header.
 *
 * @date      20241003
 * @author    Maximilian Seesslen <mes@seesslen.net>
 * @copyright SPDX-License-Identifier: Apache-2.0
 *
 *--------------------------------------------------------------------------*/


/*--- Includes -------------------------------------------------------------*/


#include <QString>
#include <QTextStream>
#include <sourceFileHeader.hpp>


/*--- Implementation -------------------------------------------------------*/


void CSourceFileHeader::writeTop( )
{
   QString macro=m_name.toUpper();
   m_stream << "#if ! defined TDT_" << macro <<"_HPP\n";
   m_stream << "#define TDT_" << macro << "_HPP\n";
   m_stream << "//---------------------------------------\n";
   m_stream << "// Generated file, do not change!\n";
   m_stream << "// tdtGen was used to create this file.\n";
   m_stream << "//---------------------------------------\n";
   m_stream << "\n";
   m_stream << "\n";
   m_stream << "// headers: " << m_includes.count() << "\n";
   if( m_includes.count() )
   {
      for( const auto& include: m_includes)
      {
         m_stream << "#include " << include << "\n";
      }
      m_stream << "\n";
      m_stream << "\n";
   }
   m_stream << "namespace Tdt\n";
   m_stream << "{\n";
   m_stream << "\n";
   m_stream << "enum class " << m_enum << "\n";
   m_stream << "{\n";
};


void CSourceFileHeader::writeBottom( )
{
   QString macro=m_name.toUpper();
   
   m_stream << "};\n";
   m_stream << "\n";
   m_stream << "} // namespace Tdt\n";
   m_stream << "\n";
   m_stream << "#endif // ! ? TDT_" << macro << "_HPP\n";
};


/*--- Fin ------------------------------------------------------------------*/
