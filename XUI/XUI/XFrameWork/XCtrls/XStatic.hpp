#pragma once
#include "../XElement.hpp"

class CXStatic : public CXElement
{
	XClass;

	XProperty_Begin
		XProperty(Text,CString)
		XProperty(TextColor,COLORREF)
	XProperty_End;
public:
	virtual XResult OnPaint();

};

MyNameIs(CXStatic)
	I_Can("ÏÔÊ¾¾²Ì¬ÎÄ±¾")
End_Description;

//////////////////////////////////////////////////////////////////////////


XResult CXStatic::OnPaint()
{
	return XResult_OK;
}

XResult CXStatic::SetText(CString param)
{
	m_Text = param;
	return XResult_OK;
}

CString CXStatic::GetText()
{
	return m_Text;
}

XResult CXStatic::SetTextColor(COLORREF param)
{
	m_TextColor = param;
	return XResult_OK;
}
COLORREF CXStatic::GetTextColor()
{
	return m_TextColor;
}