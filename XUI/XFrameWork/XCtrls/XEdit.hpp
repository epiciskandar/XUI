#pragma once
#include "../XElement.h"
#include "XFont.hpp"

// 每次输入时都能有个反馈?比如闪烁

class CXEdit : public CXElement , virtual public IXText
	, public CWindowImpl<CXEdit,CEdit>
{
	RefCountImplAt(CXElement);
public:
	XFakeProperty_Set(Text);
	XProperty_Get(Text);
	XProperty(TextColor);
	XProperty(XFont);

	BEGIN_MSG_MAP_EX(CXEdit)
	END_MSG_MAP()

	virtual XResult ProcessXMessage(IXMsg& msg) override;

public:
	VOID On_CXMsg_AttachDC(CXMsg_AttachDC& arg);
	VOID _Create(HWND hWndParent);
};

//////////////////////////////////////////////////////////////////////////

inline XResult CXEdit::ProcessXMessage( IXMsg& msg )
{
	__super::ProcessXMessage(msg);
	
	BEGIN_XMSG_MAP(msg)
		OnXMsg(CXMsg_AttachDC);
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
	GetRectInClientCoord(rect);
	Create(hWndParent,rect,nullptr,WS_CHILD);
	CString text;
	GetText(text);
	SetWindowText(text);
	ShowWindow(SW_SHOW);
}

XResult CXEdit::SetText(Property::TextType param)
{
	m_property->SetProperty(Property::Text, param);
	if (!IsWindow())
	{
		return XResult_NotHandled;
	}
	SetWindowText(param);
	return XResult_OK;
}
