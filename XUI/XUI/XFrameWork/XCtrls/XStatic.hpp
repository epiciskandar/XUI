#pragma once
#include "../XElement.hpp"

class CXStatic : public CXElement
{
	XClass;

	XProperty_Begin
		XProperty(Text,CString)
		XProperty(TextColor,COLORREF)
	XProperty_End;

	BEGIN_MSG_MAP_EX(CXStatic)
		On_XMessage(CXMsg_Paint)
	END_MSG_MAP()
public:
	LRESULT On_CXMsg_Paint(CXMsg_Paint* arg);
};

typedef XSmartPtr<CXStatic> CXStaticRef;

MyNameIs(CXStatic)
	I_Can("ÏÔÊ¾¾²Ì¬ÎÄ±¾")
End_Description;

//////////////////////////////////////////////////////////////////////////

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

LRESULT On_CXMsg_Paint(CXMsg_Paint* arg)
{
	return 0;
}