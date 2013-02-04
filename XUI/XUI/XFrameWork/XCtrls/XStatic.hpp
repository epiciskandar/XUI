#pragma once
#include "../XElement.hpp"

class CXStatic : public CXElement
{
	XClass(CXElement);

	XProperty_Begin
		XProperty(Text)
		XProperty(TextColor)
	XProperty_End;

	virtual XResult RegisterXMLSupportProperty();

	virtual XResult ProcessXMessage(CXMsg& msg);

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

XResult CXStatic::ProcessXMessage( CXMsg& msg )
{
	BEGIN_XMSG_MAP(msg)
		OnXMsg(CXMsg_Paint);
	END_XMSG_MAP;

	return BaseClass::ProcessXMessage(msg);
}

XResult CXStatic::RegisterXMLSupportProperty()
{
	XMLConvert(Text);
	XMLConvert(TextColor);

	return BaseClass::RegisterXMLSupportProperty();
}
