#pragma once
#include "XBase.hpp"
#include "XTree.hpp"
#include "XMsg.hpp"
#include "XProperty.hpp"

class CXElement : public CXBase
	, public CXTreeNode
{
	XClass;

	XProperty_Begin
		XProperty(m_property,CString,	ID)
		XProperty(m_property,CRect,		Rect)
		XProperty(m_property,CPoint,	Position)
		XProperty(m_property,CPoint,	Size)
	XProperty_End;

	SupportXMessage;
	CXProperty& GetPrpertyRef(){return m_property;};

public:
	CXElement();
	~CXElement();
protected:
	VOID _SendXMessageToChildren(CXMsg& pMsg);
	LRESULT On_CXMsg_Paint(CXMsg_Paint& arg);
protected:
	CXProperty m_property;
};

DefineRef(CXElement);

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
		CXElementRef pElement = TransformNode<CXElementRef>(*i);
		pElement->ProcessXMessage( pMsg );
	}
}
