#pragma once
#include "../XElement.h"
#include "XFont.hpp"

class CXText : public CXElement , virtual public IXText
{
	RefCountImplAt(CXElement);
public:
	XProperty(Text);
	XProperty(TextColor);
	XProperty(XFont);

	virtual XResult ProcessXMessage(IXMsg& msg) override;

public:
	VOID On_CXMsg_Paint(CXMsg_Paint& msg);
};

//////////////////////////////////////////////////////////////////////////

inline VOID CXText::On_CXMsg_Paint(CXMsg_Paint& msg)
{
	XMsgTraceWithID(msg);

	__super::On_CXMsg_Paint(msg);

	BOOL bGhost = FALSE;
	GetGhost(bGhost);
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

inline XResult CXText::ProcessXMessage( IXMsg& msg )
{
	__super::ProcessXMessage(msg);
	
	BEGIN_XMSG_MAP(msg)
		OnXMsg(CXMsg_Paint);
	END_XMSG_MAP;

	return XResult_OK;
}
