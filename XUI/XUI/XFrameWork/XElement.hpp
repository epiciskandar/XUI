#pragma once
#include "XBase.hpp"
#include "XTree.hpp"
#include "XMsg.hpp"
#include "XProperty.hpp"
#include "XGaia.hpp"
#include "Utils/XMLConverter.hpp"

//////////////////////////////////////////////////////////////////////////

#define XProperty_Begin
#define XProperty_Get(_paramtype,_name) \
	virtual XResult Get##_name (_paramtype& value) \
	{ \
		return m_property.GetProperty(L#_name,value); \
	}
#define XProperty_Set(_paramtype,_name) \
public: \
	virtual XResult Set##_name (_paramtype param) \
	{ \
		return m_property.SetProperty(L#_name,param); \
	}
#define XProperty(_paramtype,_name) \
	XProperty_Get(_paramtype,_name) \
	XProperty_Set(_paramtype,_name)

#define XProperty_End

#define XMLConvert_Begin virtual XResult RegisterXMLSupportProperty(){
#define XMLConvert(_name,_converter) \
	m_XMLConverterMap[L# _name] = [this](CString value)->XResult \
	{ \
		_converter converter(m_property); \
		return converter.Convert(L# _name,value); \
	};
#define XMLConvert_End(_baseClass) return _baseClass::RegisterXMLSupportProperty();}

//////////////////////////////////////////////////////////////////////////

class CXElement : public CXBase
	, public CXTreeNode
{
	XClass;

	XProperty_Begin
		XProperty(CRect,	Rect)
		XProperty(CPoint,	Position)
		XProperty(CPoint,	Size)
	XProperty_End;

	SupportXMessage;

public:
	CXElement();
	~CXElement();
	virtual XResult RegisterXMLSupportProperty()
	{
		XMLConvert(Rect,		XMLConverter_CRect);
		XMLConvert(Position,	XMLConverter_CPoint);
		XMLConvert(Size,		XMLConverter_CPoint);
		return XResult_OK;
	}
	virtual XResult SetXMLProperty(CString name,CString value);

	CXProperty& GetPrpertyRef()
	{
		return m_property;
	};

protected:
	VOID _SendXMessageToChildren(CXMsg& pMsg);
	LRESULT On_CXMsg_Paint(CXMsg_Paint& arg);
protected:
	CXProperty m_property;
	typedef std::map<CString,std::function<XResult(CString)>> XMLConverterMap;
	XMLConverterMap m_XMLConverterMap;
};

typedef XSmartPtr<CXElement> ElementRef;

MyNameIs(CXElement)
	I_Provide("可绘制元素的基本属性及X消息路由")
End_Description;

//////////////////////////////////////////////////////////////////////////

CXElement::CXElement()
{
}

CXElement::~CXElement()
{

}

LRESULT CXElement::On_CXMsg_Paint(CXMsg_Paint& arg)
{
	if (!arg.msgHandled)
	{
		_SendXMessageToChildren(arg);
	}
	return XResult_OK;
}

VOID CXElement::_SendXMessageToChildren( CXMsg& pMsg )
{
	for (auto i=m_children.begin(); i!=m_children.end(); ++i)
	{
		XSmartPtr<CXElement> pElement = TransformNode<XSmartPtr<CXElement>>(*i);
		pElement->ProcessXMessage( pMsg );
	}
}

XResult CXElement::SetXMLProperty( CString name,CString value )
{
	auto i = m_XMLConverterMap.find(name);
	if (i != m_XMLConverterMap.end())
	{
		return i->second(value);
	}
	return XResult_NotSupport;
}
