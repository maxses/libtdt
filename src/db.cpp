//-----------------------------------------------------------------------------
//
//
//
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------


#include <lepto/can_tdt_db.hpp>
#include <QFile>
#include <QStandardPaths>
//#include <QXmlStreamReader>
#include <QDebug>
#include <QDir>
#include <QtXml/QDomDocument>


//-----------------------------------------------------------------------------


namespace Tdt
{

CCanTdtDb::CCanTdtDb()
{
   QFile file( QDir::home().path() + "/.lepto/tdt.xml" );
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
      qWarning() << "Install a proper device info XML file to $HOME/.lepto/tdt.xml";
      return;
	exit(22);
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
               QString nodeSubId=subNode.attribute("nodeSubId","0");
               int nodeSubId_int=nodeSubId.toInt(nullptr,0);
               qWarning() << "   nodeSubId: " << nodeSubId_int << "; " << subNode.attribute("name","");
               m_nodeSubIdNames[ EID(nodeId_int, nodeSubId_int ) ] = subNode.attribute("name","");
               
               QString objectString=subNode.attribute("object","0");
               int objectInt=objectString.toInt(nullptr,0);
               if(objectInt)
               {
                  m_nodeObjectNames[ EID(nodeId_int, objectInt) ] = subNode.attribute("name","");
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


//---fin-----------------------------------------------------------------------
