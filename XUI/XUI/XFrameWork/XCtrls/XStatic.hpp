#pragma once
#include "../XElement.hpp"

class CXStatic : public CXElement
{
	XClass;

	XProperty_Begin
		XProperty(CString,	Text)
		XProperty(COLORREF,	TextColor)
	XProperty_End;

	XMLConvert_Begin
		XMLConvert(Text,		XMLConverter_CString)
		XMLConvert(TextColor,	XMLConverter_CPoint)
	XMLConvert_End(CXElement)

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

VOID CXStatic::On_CXMsg_Paint(CXMsg_Paint& msg)
{
	CRect rect;
	GetRect(rect);
	CString text;
	GetText(text);
	if (msg.drawDevice.IsRectNeedRePaint(rect))
	{
		DRAWTEXTPARAMS params;
		ZeroMemory(&params,sizeof(params));
		params.cbSize = sizeof(params);
		DrawTextEx(msg.drawDevice.dc,
			text.GetBuffer(text.GetLength()),
			text.GetLength(),
			rect,DT_CENTER | DT_SINGLELINE | DT_VCENTER,
			&params);
		text.ReleaseBuffer();
		_SendXMessageToChildren(msg);
	}
	msg.msgHandled = TRUE;
}