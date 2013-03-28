#pragma once
#include "../XElement.hpp"

class CXStatic : public CXElement
{
	XClass(CXElement);

	XProperty_Begin
		XProperty(Text)
		XProperty(TextColor)
        XProperty(FontName)
        XProperty(FontSize)
	XProperty_End;

	virtual XResult SetXMLProperty( CString name,CString value );

	virtual XResult ProcessXMessage(CXMsg& msg);

public:
	VOID On_CXMsg_Paint(CXMsg_Paint& msg);
};

typedef XSmartPtr<CXStatic> CXStaticRef;

MyNameIs(CXStatic)
	I_Can("显示静态文本")
End_Description;

//////////////////////////////////////////////////////////////////////////

inline VOID CXStatic::On_CXMsg_Paint(CXMsg_Paint& msg)
{
	XMsgTraceID(msg);

	BaseClass::On_CXMsg_Paint(msg);

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
    CString fontName;
    GetFontName(fontName);
    INT fontSize;
    GetFontSize (fontSize);

	if (msg.drawDevice.IsRectNeedRePaint(rect))
	{
		COLORREF color;
		GetColor(color);
		CPen pen;
        pen.CreatePen(PS_SOLID,1,color);
        
        if (fontName != _T(""))
        {
            /* 修改字体 */
            CFont newFont;
            newFont.CreatePointFont (fontSize*10, fontName);         
            msg.drawDevice.dc.SelectFont (newFont.m_hFont);
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
	BaseClass::ProcessXMessage(msg);
	
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
        XMLConvert(FontName)
        XMLConvert(FontSize)
	XMLConvert_End

	return BaseClass::SetXMLProperty(name,value);
}
