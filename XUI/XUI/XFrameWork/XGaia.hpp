#pragma once
#include "XTree.hpp"
#include <map>
#include <functional>

#include "XFrameWork/XFrame.hpp"
#include "XFrameWork/XRealWnd.hpp"
#include "XFrameWork/XCtrls/XStatic.hpp"

#include "../TinyXML/tinyxml.h"

class CXGaia
{
	XClass(VOID);
	XSingleton(CXGaia);
public:
	NodeRef Create(CString className);
	NodeRef CreateFromXML(CString xmlFile);
	NodeRef ParseXMLNode(TiXmlElement* pElement);
	XResult ParseAndSetParams(NodeRef node,const TiXmlElement* pElement);

protected:
	XResult _CheckAndSkipXMLRoot(TiXmlElement* pRoot);
protected:
	typedef std::map<CString,std::function<NodeRef()>> ElementRecord;
	ElementRecord m_record;
};

MyNameIs(CXGaia)
	I_Can("创建子节点")
End_Description;

//////////////////////////////////////////////////////////////////////////

#define XUIXMLFlag		"XUI"
#define XUIXMLVersion	"1"

#define RecordXClass(xclass) m_record[xclass::GetMyClassName()] = []{return new xclass;};

CXGaia::CXGaia()
{
	RecordXClass(CXRealWnd);
	RecordXClass(CXStatic);
}

NodeRef CXGaia::Create( CString className )
{
	auto i = m_record.find(className);
	if (i != m_record.end())
	{
		return NodeRef((i->second)());	// it is important to init NodeRef this way
	}
	return nullptr;
}

NodeRef CXGaia::CreateFromXML( CString xmlFile )
{
	CStringA xmlFileA(xmlFile);
	TiXmlDocument doc;
	bool result = doc.LoadFile(xmlFileA);
	if (result)
	{
		TiXmlElement* pRoot = doc.RootElement();
		ATLASSERT(pRoot && "invalid xml content");
		if (pRoot)
		{
			XResult bRet = _CheckAndSkipXMLRoot(pRoot);
			if (XSUCCEEDED(bRet))
			{
				TiXmlNode* childNode = pRoot->FirstChild();
				if (childNode)
				{
					pRoot = childNode->ToElement();
					if (pRoot)
					{
						NodeRef rootNode = ParseXMLNode(pRoot);
						return rootNode;
					}
				}
			}
		}
	}
	return nullptr;
}

NodeRef CXGaia::ParseXMLNode( TiXmlElement* pElement )
{
	CStringA strClass = pElement->Value();
	NodeRef node;
	CString strClassName = _T("C");
	strClassName += strClass;
	node = Create(strClassName);
	if (!node)
	{
		return nullptr;
	}
	ParseAndSetParams(node,pElement);
	TiXmlNode* pChild = pElement->FirstChild();
	if (pChild)
	{
		TiXmlElement* pChildElement = pChild->ToElement();
		if (pChildElement)
		{
			NodeRef childNode = ParseXMLNode(pChildElement);
			if (childNode)
			{
				node->AppendChild(childNode);
			}
		}
	}
	return node;
}

XResult CXGaia::_CheckAndSkipXMLRoot( TiXmlElement* pRoot )
{
	CheckParam(pRoot);
	CStringA nodeValue = pRoot->Value();
	if (nodeValue == XUIXMLFlag)
	{
		LPCSTR version = pRoot->Attribute("version");
		if (version)
		{
			CStringA strVersion(version);
			if (strVersion == XUIXMLVersion)
			{
				return XResult_OK;
			}
		}
	}
	return XResult_Error;
}

XResult CXGaia::ParseAndSetParams( NodeRef node,const TiXmlElement* pElement )
{
	CheckParam(node && pElement);
	ElementRef element = TransformNode<ElementRef>(node);
	if (element)
	{
		const TiXmlAttribute* attr = pElement->FirstAttribute();
		while (attr)
		{
			CString key = attr->Name();
			CString value = attr->Value();
			element->SetXMLProperty(key,value);
			attr = attr->Next();
		}
	}
	return XResult_OK;
}

