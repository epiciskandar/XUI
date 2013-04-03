#pragma once
#include "XTree.hpp"
#include <map>
#include <functional>

#include "XFrameWork/XFrame.hpp"
#include "XFrameWork/XRealWnd.hpp"
#include "XFrameWork/XCtrls/XStatic.hpp"
#include "XFrameWork/XCtrls/XImage.hpp"

#include "../TinyXML/tinyxml.h"

typedef std::function<XResult(NodeRef node)>	ListenerRegister;

class CXGaia
{
	XClass(VOID);
	XSingleton(CXGaia);
public:
	NodeRef Create(CString className);
	NodeRef CreateFromXML(CString xmlFile);
	NodeRef ParseXMLNode(TiXmlElement* pElement);
	XResult ParseAndSetParams(NodeRef node,const TiXmlElement* pElement);
	XResult ParseAndSetProperter(NodeRef properter,const TiXmlElement* proplement);
	XResult SetListenerRegister(ListenerRegister reger);

protected:
	XResult _CheckAndSkipXMLRoot(TiXmlElement* pRoot);
protected:
	typedef std::map<CString,std::function<NodeRef()>> ElementRecord;
	ElementRecord m_record;
	ListenerRegister	m_listenRegisterFunc;
};

MyNameIs(CXGaia)
	I_Can("创建子节点")
End_Description;

//////////////////////////////////////////////////////////////////////////

#define XUIXMLFlag		"XUI"
#define XUIXMLVersion	"1"
#define SubPropertyNodeName	"Property"

#define RecordXClass(xclass) m_record[xclass::GetMyClassName()] = []{return new xclass;};

inline CXGaia::CXGaia()
{
	RecordXClass(CXElement);
	RecordXClass(CXRealWnd);
	RecordXClass(CXStatic);
	RecordXClass(CXImage);
	RecordXClass(CXFont);
}

inline NodeRef CXGaia::Create( CString className )
{
	auto i = m_record.find(className);
	if (i != m_record.end())
	{
		return NodeRef((i->second)());	// it is important to init NodeRef this way
	}
	else
	{
		WTF;
	}
	return nullptr;
}

inline NodeRef CXGaia::CreateFromXML( CString xmlFile )
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

inline NodeRef CXGaia::ParseXMLNode( TiXmlElement* pElement )
{
	CStringA strClass = pElement->Value();
	if (strClass == SubPropertyNodeName)
	{
		return nullptr;
	}
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
	while (pChild)
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
		pChild = pChild->NextSiblingElement();
	}
	if (m_listenRegisterFunc)
	{
		m_listenRegisterFunc(node);
	}
	return node;
}

inline XResult CXGaia::_CheckAndSkipXMLRoot( TiXmlElement* pRoot )
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

inline XResult CXGaia::ParseAndSetParams( NodeRef node,const TiXmlElement* pElement )
{
	CheckParam(node && pElement);
	XSmartPtr<CXElement> element = node;
	if (element)
	{
		const TiXmlAttribute* attr = pElement->FirstAttribute();
		while (attr)
		{
			CString key = attr->Name();
			CStringA valueA = attr->Value();
			CString value;
			Util::String::UTF8ToUnicode(valueA,value);
			element->SetXMLProperty(key,value);
			attr = attr->Next();
		}
		const TiXmlNode* propNode = pElement->FirstChild(CStringA(SubPropertyNodeName));
		if (propNode)
		{
			const TiXmlElement* propElement = propNode->ToElement();
			const TiXmlNode* childNode = propElement->FirstChild();
			const TiXmlElement* childElement = nullptr;
			if (childNode)
			{
				childElement = childNode->ToElement();
			}
			while (childElement)
			{
				CString name = childElement->Value();
				name = _T("C") + name;
				NodeRef propNode = Create(name);
				if (propNode)
				{
					ParseAndSetProperter(propNode,childElement);
				}
				childElement = childElement->NextSiblingElement();
			}
		}
	}
	return XResult_OK;
}

inline XResult CXGaia::SetListenerRegister( ListenerRegister reger )
{
	m_listenRegisterFunc = reger;
	return XResult_OK;
}

inline XResult CXGaia::ParseAndSetProperter( NodeRef properterNode,const TiXmlElement* proplement )
{
	const TiXmlElement* pchildElement = proplement->ToElement();
	const TiXmlAttribute* pchildAttr = pchildElement->FirstAttribute();
	while (pchildAttr)
	{
		CString key = pchildAttr->Name();
		CStringA valueA = pchildAttr->Value();
		CString value;
		Util::String::UTF8ToUnicode(valueA,value);

		XSmartPtr<CXProperter> properter(properterNode);
		if (properter)
		{
			properter->SetXMLProperty(key,value);
		}
		pchildAttr = pchildAttr->Next();
	}
	return XResult_OK;
}
