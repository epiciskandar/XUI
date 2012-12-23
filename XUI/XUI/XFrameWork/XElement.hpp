#pragma once
#include "XBase.hpp"
#include "XTree.hpp"

class CXElement : public CXBase
	, public CXTreeNode
{
	XClass;

	XProperty_Begin
		XProperty(ID,CString)
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
