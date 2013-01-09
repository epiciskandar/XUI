#pragma once
#include "../XElement.hpp"

class CXStatic : public CXElement
{
	XClass;

	XProperty_Begin
		XProperty(Text,CString)
		XProperty(TextColor,COLORREF)
	XProperty_End;

	BEGIN_XMESSAGE_MAP
		On_XMessage(CXMsg_Paint)
	END_XMESSAGE_MAP;
public:
	VOID On_CXMsg_Paint(CXMsg_Paint& msg);
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

VOID CXStatic::On_CXMsg_Paint(CXMsg_Paint& msg)
{
	if (msg.drawDevice.IsRectNeedRePaint(m_Rect))
	{
		DRAWTEXTPARAMS params;
		ZeroMemory(&params,sizeof(params));
		params.cbSize = sizeof(params);
		DrawTextEx(msg.drawDevice.dc,
			m_Text.GetBuffer(m_Text.GetLength()),
			m_Text.GetLength(),
			m_Rect,DT_CENTER | DT_SINGLELINE | DT_VCENTER,
			&params);
		m_Text.ReleaseBuffer();
		_SendXMessageToChildren(msg);
	}
	msg.msgHandled = TRUE;
}