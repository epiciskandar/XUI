#pragma once
#include "XBase.hpp"
#include "XTree.hpp"
#include "XMsg.hpp"

class CXElement : public CXBase
	, public CXTreeNode
{
	XClass;

	XProperty_Begin
		XProperty(ID,CString)
		XProperty(Rect,CRect)
		XProperty_Interface(Position,CPoint)
		XProperty_Interface(Size,CPoint)
	XProperty_End;

	SupportXMessage;

public:
	CXElement();
	~CXElement();
protected:
	VOID _SendXMessageToChildren(CXMsg& pMsg);
protected:
	LRESULT On_CXMsg_Paint(CXMsg_Paint& arg);
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

XResult CXElement::SetID(CString param)
{
	m_ID = param;
	return XResult_OK;
}

CString CXElement::GetID()
{
	return m_ID;
}

XResult CXElement::SetRect(CRect param)
{
	m_Rect = param;
	return XResult_OK;
}

CRect CXElement::GetRect()
{
	return m_Rect;
}

XResult CXElement::SetPosition(CPoint param)
{
	m_Rect.TopLeft() = param;
	return XResult_OK;
}

CPoint CXElement::GetPosition()
{
	return CPoint(m_Rect.TopLeft());
}

XResult CXElement::SetSize(CPoint param)
{
	m_Rect.BottomRight() = param;
	return XResult_OK;
}

CPoint CXElement::GetSize()
{
	return CPoint(m_Rect.BottomRight());
}

VOID CXElement::_SendXMessageToChildren( CXMsg& pMsg )
{
	for (auto i=m_children.begin(); i!=m_children.end(); ++i)
	{
		CXElementRef pElement = TransformNode<CXElementRef>(*i);
		pElement->ProcessXMessage( pMsg );
	}
}
