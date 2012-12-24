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

public:
	CXElement();
	~CXElement();
public:
	virtual XResult OnPaint();
};

MyNameIs(CXElement)
End_Description;

//////////////////////////////////////////////////////////////////////////

CXElement::CXElement()
{

}

CXElement::~CXElement()
{

}

XResult CXElement::OnPaint()
{
	for (auto i=m_children.begin(); i!=m_children.end(); ++i)
	{
	}
	return XResult_NotImpl;
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
