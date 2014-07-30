#pragma once
#include "XTree.hpp"
#include "XFrameWork/IXMLParsable.h"
#include <map>

#include "XRealWnd.h"
#include "XCtrls/XStatic.hpp"
#include "XCtrls/XImage.hpp"
#include "XCtrls/XEdit.hpp"

#include "Third/TinyXML/tinyxml.h"

class CXGaia : public IXGaia
{
	XSingleton(CXGaia);
public:
	XNodeRef Create(CString className) override;
	XNodeRef CreateFromXML(CString xmlFile) override;
	XResult RegListener(ListenerRegister reger) override;
	XNodeRef ParseXMLNode(TiXmlElement* pElement);
	XResult ParseAndSetParams(XNodeRef node,const TiXmlElement* pElement);
	XResult ParseAndSetProperter(XNodeRef properter,const TiXmlElement* proplement);

protected:
	XResult _CheckAndSkipXMLRoot(TiXmlElement* pRoot);
protected:
	typedef std::map<CString,std::function<XNodeRef()>> ElementRecord;
	ElementRecord m_record;
	ListenerRegister	m_listenRegisterFunc;
};

//////////////////////////////////////////////////////////////////////////

#define XUIXMLFlag		"XUI"
#define XUIXMLVersion	"1"
#define SubPropertyNodeName	"Property"

#define RecordXClass(xclass) m_record[typeid(xclass).name()] = []{return new xclass;};

inline CXGaia::CXGaia()
{
	RecordXClass(CXElement);
	RecordXClass(CXRealWnd);
	RecordXClass(CXText);
	RecordXClass(CXImage);
	RecordXClass(CXFont);
	RecordXClass(CXEdit);
}

inline XNodeRef CXGaia::Create( CString className )
{
	className = _T("class ")+className;
	auto i = m_record.find(className);
	if (i != m_record.end())
	{
		return XNodeRef((i->second)());	// it is important to init XNodeRef this way
	}
	else
	{
		WTF;	// no such element
	}
	return nullptr;
}

inline XNodeRef CXGaia::CreateFromXML( CString xmlFile )
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
						XNodeRef rootNode = ParseXMLNode(pRoot);
						return rootNode;
					}
				}
			}
		}
	}
	return nullptr;
}

inline XNodeRef CXGaia::ParseXMLNode( TiXmlElement* pElement )
{
	CStringA strClass = pElement->Value();
	if (strClass == SubPropertyNodeName)
	{
		return nullptr;
	}
	XNodeRef node;
	CString strClassName = _T("CX");
	strClassName += strClass;
	node = Create(strClassName);
	if (!node)
	{
		return nullptr;
	}
	XPtr<IXMLParsable> pParsableObj(node);
	if (pParsableObj)
	{
		pParsableObj->ParseXml(pElement);
	}
	else
	{
		ParseAndSetParams(node, pElement);
		TiXmlNode* pChild = pElement->FirstChild();
		while (pChild)
		{
			TiXmlElement* pChildElement = pChild->ToElement();
			if (pChildElement)
			{
				XNodeRef childNode = ParseXMLNode(pChildElement);
				if (childNode)
				{
					node->AppendChild(childNode);
				}
			}
			pChild = pChild->NextSiblingElement();
		}
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

inline XResult CXGaia::ParseAndSetParams( XNodeRef node,const TiXmlElement* pElement )
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
				CString className = _T("CX") + name;
				XNodeRef propNode = Create(className);
				if (propNode)
				{
					ParseAndSetProperter(propNode,childElement);
					IXProperty* prop=nullptr;
					element->GetProperty(prop);
					if (prop)
					{
						prop->SetProperty(name,propNode);
					}
				}
				childElement = childElement->NextSiblingElement();
			}
		}
	}
	return XResult_OK;
}

inline XResult CXGaia::RegListener( ListenerRegister reger )
{
	m_listenRegisterFunc = reger;
	return XResult_OK;
}

inline XResult CXGaia::ParseAndSetProperter( XNodeRef node,const TiXmlElement* proplement )
{
	const TiXmlElement* pchildElement = proplement->ToElement();
	const TiXmlAttribute* pchildAttr = pchildElement->FirstAttribute();
	while (pchildAttr)
	{
		CString key = pchildAttr->Name();
		CStringA valueA = pchildAttr->Value();
		CString value;
		Util::String::UTF8ToUnicode(valueA,value);

		IXMLPropertyParsable* properter = dynamic_cast<IXMLPropertyParsable*>(node.GetPointer());
		if (properter)
		{
			properter->SetXMLProperty(key,value);
		}
		pchildAttr = pchildAttr->Next();
	}
	return XResult_OK;
}
