#pragma once
#include "../XElement.h"
#include "XFont.hpp"

class CXEdit : public CXElement , virtual public IXText
	, public CWindowImpl<CXEdit,CEdit>
{
	XClass;
public:
	XProperty(Text);
	XProperty(TextColor);
	XProperty(XFont);

	BEGIN_MSG_MAP_EX(CXEdit)
		//MSG_WM_ERASEBKGND(OnEraseBkgnd)
		//MSG_WM_PAINT(OnPaint)
		COMMAND_CODE_HANDLER(EN_CHANGE,OnEnChanged)
	END_MSG_MAP()

	virtual XResult ProcessXMessage(IXMsg& msg) override;

public:
	VOID On_CXMsg_PropertyChanged(CXMsg_PropertyChanged& arg);
	VOID On_CXMsg_Paint(CXMsg_Paint& msg);
	VOID On_CXMsg_AttachDC(CXMsg_AttachDC& arg);
	VOID On_CXMsg_OnCtlColor(CXMsg_OnCtlColor& arg);
	VOID _Create(HWND hWndParent);
	BOOL OnEraseBkgnd(HDC hDC);
	VOID OnPaint(HDC hDC);
	LRESULT OnEnChanged(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
};

//////////////////////////////////////////////////////////////////////////

inline VOID CXEdit::On_CXMsg_Paint(CXMsg_Paint& msg)
{
	XMsgTraceID(msg);

	__super::On_CXMsg_Paint(msg);

	msg.msgHandled = TRUE;
}

inline XResult CXEdit::ProcessXMessage( IXMsg& msg )
{
	__super::ProcessXMessage(msg);
	
	BEGIN_XMSG_MAP(msg)
		OnXMsg(CXMsg_PropertyChanged);
		OnXMsg(CXMsg_Paint);
		OnXMsg(CXMsg_AttachDC);
		OnXMsg(CXMsg_OnCtlColor);
	END_XMSG_MAP;

	return XResult_OK;
}

VOID CXEdit::On_CXMsg_AttachDC( CXMsg_AttachDC& arg )
{
	if (arg.hostWnd)
	{
		if (!IsWindow())
		{
			_Create(arg.hostWnd);
		}
		else
		{
			Attach(arg.hostWnd);
		}
	}
	else
	{
		if (IsWindow())
		{
			Detach();
		}
	}
}

VOID CXEdit::_Create(HWND hWndParent)
{
	CRect rect;
	CXElement::GetRect(rect);
	rect = ElementUtil::GetElementRectInClientCoord(this);
	Create(hWndParent,rect,nullptr,WS_CHILD);
	CXMsg_PropertyChanged msg;
	msg.name = Property::Text;
	On_CXMsg_PropertyChanged(msg);
	ShowWindow(SW_SHOW);
}

VOID CXEdit::On_CXMsg_PropertyChanged( CXMsg_PropertyChanged& arg )
{
	if (!IsWindow())
	{
		return;
	}
	if (arg.name == Property::Text)
	{
		CString text;
		GetText(text);
		SetWindowText(text);
	}
	else if (arg.name == Property::LayoutRect)
	{
		CRect rect;
		GetLayoutRect(rect);
		rect = rect;
	}
}

VOID CXEdit::On_CXMsg_OnCtlColor( CXMsg_OnCtlColor& arg )
{
	if (arg.hWnd != m_hWnd)	WTF;

	SetBkMode(arg.hDC,TRANSPARENT);
	arg.hBrush = GetStockObject(HOLLOW_BRUSH);

	arg.msgHandled = TRUE;
}

BOOL CXEdit::OnEraseBkgnd( HDC hDC )
{
	URP(hDC);
	SetBkMode(hDC,TRANSPARENT);
	return TRUE;
}

VOID CXEdit::OnPaint( HDC hDC )
{
	URP(hDC);
	hDC = hDC;
}

LRESULT CXEdit::OnEnChanged( WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled )
{
	URP(wNotifyCode,wID,hWndCtl,bHandled);
	return 0;
}
