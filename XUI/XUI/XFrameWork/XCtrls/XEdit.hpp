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
	END_MSG_MAP()

	virtual XResult ProcessXMessage(IXMsg& msg) override;

public:
	VOID On_CXMsg_PropertyChanged(CXMsg_PropertyChanged& arg);
	VOID On_CXMsg_AttachDC(CXMsg_AttachDC& arg);
	VOID _Create(HWND hWndParent);
};

//////////////////////////////////////////////////////////////////////////

inline XResult CXEdit::ProcessXMessage( IXMsg& msg )
{
	__super::ProcessXMessage(msg);
	
	BEGIN_XMSG_MAP(msg)
		OnXMsg(CXMsg_PropertyChanged);
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
}
