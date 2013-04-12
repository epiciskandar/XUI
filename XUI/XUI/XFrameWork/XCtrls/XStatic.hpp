#pragma once
#include "../XElement.h"
#include "XFont.hpp"

class CXStatic : public CXElement
{
	XClass;
public:
	XProperty_Begin
		XProperty(Text)
		XProperty(TextColor)
        XProperty(XFont)
	XProperty_End;

	virtual XResult SetXMLProperty( CString name,CString value );

	virtual XResult ProcessXMessage(CXMsg& msg);

public:
	VOID On_CXMsg_Paint(CXMsg_Paint& msg);
};

typedef XSmartPtr<CXStatic> CXStaticRef;

//////////////////////////////////////////////////////////////////////////

inline VOID CXStatic::On_CXMsg_Paint(CXMsg_Paint& msg)
{
	XMsgTraceID(msg);

	__super::On_CXMsg_Paint(msg);

	BOOL bGhost = FALSE;
	GetGhost (bGhost);
	if (bGhost) //Ghost 属性为真 跳过绘制
	{
		return;
	}

	CRect rect;
	GetRect(rect);
	rect.OffsetRect(msg.offsetFix);
	CString text;
	GetText(text);


	if (msg.drawDevice.IsRectNeedRePaint(rect))
	{
		COLORREF color;
		GetColor(color);
		CPen pen;
        pen.CreatePen(PS_SOLID,1,color);
        
		XNodeRef fontNode;
		GetXFont(fontNode);
		XPtr<CXFont> font(fontNode);
        if (font && font->GetFontName() != _T(""))
        {
            /* 修改字体 */
            font->ChangeWork();    
            msg.drawDevice.dc.SelectFont (font->m_hFont);
         }


		CGDIHandleSwitcher switcher(msg.drawDevice.dc,pen,FALSE);
		DRAWTEXTPARAMS params;
		ZeroMemory(&params,sizeof(params));
		params.cbSize = sizeof(params);
		msg.drawDevice.dc.DrawTextEx(
			text.GetBuffer(text.GetLength()),
			text.GetLength(),
			rect,DT_CENTER | DT_SINGLELINE | DT_VCENTER,
			&params);
		text.ReleaseBuffer();
	}
	msg.msgHandled = TRUE;
}

inline XResult CXStatic::ProcessXMessage( CXMsg& msg )
{
	__super::ProcessXMessage(msg);
	
	BEGIN_XMSG_MAP(msg)
		OnXMsg(CXMsg_Paint);
	END_XMSG_MAP;

	return XResult_OK;
}

inline XResult CXStatic::SetXMLProperty( CString name,CString value )
{
	XMLConvert_Begin(name,value)
		XMLConvert(Text)
		XMLConvert(TextColor)
	XMLConvert_End

	return __super::SetXMLProperty(name,value);
}
