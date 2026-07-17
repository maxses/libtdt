/**---------------------------------------------------------------------------
 *
 * @file    db.cpp
 * @brief   Hold lists of available nodes and their objects (optional)
 *
 * This can help GUIs to show preprocessed and nicely readable tables/forms.
 * The information is read at start from $HOME/.config/tdt.xml.
 * 
 * This class is only available on the host pc, not the MCU.
 *
 * @date      20241219
 * @author    Maximilian Seesslen <src@seesslen.net>
 * @copyright SPDX-License-Identifier: Apache-2.0
 *
 *--------------------------------------------------------------------------*/


/*--- Defines --------------------------------------------------------------*/


#include <tdt/db.hpp>
#include <QFile>
#include <QDomDocument>
#include <QDebug>
#include <QDir>            // home()
#include <QDebug>


/*--- Implementation -------------------------------------------------------*/


namespace Tdt
{


CDb::CDb()
{
   QFile file( QDir::home().path() + "/.config/tdt.xml" );
   QDomDocument xmlBOM;
   
   if( !file.open(QIODevice::ReadOnly) )
   {
      qWarning("Could not open tdt file");
      return;
   }
   
   xmlBOM.setContent(&file);
   file.close();
   
   QDomElement root=xmlBOM.documentElement();
   
   if( root.tagName() != QString("tdt") )
   {
      qWarning() << "Schema wrong. Unexpercted '" << root.tagName() << "'";
      qWarning() << "Install a proper device info XML file to $HOME/.config/tdt.xml";
      qFatal( "exiting..." );
   }
   
   QDomElement node=root.firstChild().toElement();
   
   while(!node.isNull())
   {
      if( node.tagName() == "node" )
      {
         QString nodeId=node.attribute("nodeId","0");
         int nodeId_int=nodeId.toInt(nullptr,0);
         qWarning() << "nodeId: " << nodeId_int << "; " << node.attribute("name","");
         m_nodeNames[ nodeId_int ] = node.attribute("name","");
         QDomElement subNode=node.firstChild().toElement();
         while(!subNode.isNull())
         {
            if( subNode.tagName() == "sub" )
            {
               QString objectString=subNode.attribute("object","0");
               int objectInt=objectString.toInt(nullptr,0);
               if(objectInt)
               {
                  m_nodeObjectNames[ combinedId(nodeId_int, objectInt) ] = subNode.attribute("name","");
                  qWarning( "   object: 0x%X; %s", objectInt
                            , qPrintable(subNode.attribute("name","") ) );
               }
            }
            subNode = subNode.nextSibling().toElement();
         }
      }
      node = node.nextSibling().toElement();
   }
   
   return;
}


} // namespace Tdt


/*--- Fin ------------------------------------------------------------------*/
