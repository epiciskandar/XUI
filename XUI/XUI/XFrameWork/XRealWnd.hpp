#pragma once
#include "XElement.hpp"

class CXRealWnd :
	public CXElement,
	public CWindowImpl<CXRealWnd>
{
	XClass(CXElement);
public:
	CXRealWnd();

	BEGIN_MSG_MAP_EX(CXRealWnd)
		MSG_WM_DESTROY(OnDestroy)
		TranslateToXMessage( MessageTranslateFunc 
		, WM_PAINT
		)
	END_MSG_MAP();

	virtual XResult ProcessXMessage(CXMsg& msg);

	XProperty_Begin
		XProperty(Title)
		XProperty(Style)
		XProperty(ExStyle)
		XProperty(HWnd)
	XProperty_End;

	virtual XResult RegisterXMLSupportProperty();

	XResult Create(HWND hwndParent=0);

protected:
	LRESULT MessageTranslateFunc(UINT uMsg, WPARAM wParam, LPARAM lParam);
	VOID	OnDestroy();

	MessageTranslater(CXMsg_Paint);

	//--------------------------------//
};

typedef XSmartPtr<CXRealWnd> CXRealWndRef;

MyNameIs(CXRealWnd)
End_Description;

//////////////////////////////////////////////////////////////////////////

CXRealWnd::CXRealWnd() : CWindowImpl()
{

}

VOID CXRealWnd::OnDestroy()
{
	PostMessage(WM_QUIT,0,0);
}

LRESULT CXRealWnd::MessageTranslateFunc( UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	LRESULT lResult=0;
	switch(uMsg)
	{
		XMsgTranslater(WM_PAINT,CXMsg_Paint);
	}

	return lResult;
}

VOID CXRealWnd::BeforeCXMsg_Paint( CXMsg_Paint& msg, WPARAM wParam, LPARAM lParam )
{	URP(wParam,lParam);
	PAINTSTRUCT ps;
	BeginPaint(&ps);
	msg.drawDevice.invalidRect = CRect(ps.rcPaint);
	msg.drawDevice.dc = ps.hdc;
}

VOID CXRealWnd::EndCXMsg_Paint( CXMsg_Paint& msg )
{
	BitBlt(msg.drawDevice.dc,
		msg.drawDevice.invalidRect.left,
		msg.drawDevice.invalidRect.top,
		msg.drawDevice.invalidRect.Width(),
		msg.drawDevice.invalidRect.Height(),
		msg.drawDevice.dc,
		0, 0,
		SRCCOPY
		);

	PAINTSTRUCT ps;
	ZeroMemory(&ps,sizeof(ps));
	ps.hdc = msg.drawDevice.dc;
	ps.rcPaint = msg.drawDevice.invalidRect;
	EndPaint(&ps);
}

XResult CXRealWnd::Create( HWND hwndParent/*=0*/ )
{
	CString title;
	GetTitle(title);
	DWORD style=0;
	GetStyle(style);
	DWORD ExStyle=0;
	GetExStyle(ExStyle);
	HWND hWnd = CWindowImpl::Create(hwndParent,NULL,title,style,ExStyle);
	SetHWnd(hWnd);
	if (hWnd)
	{
		return XResult_OK;
	}
	return XResult_Fail;
}

XResult CXRealWnd::RegisterXMLSupportProperty()
{
	XMLConvert(Title);
	XMLConvert(ShowState);
	XMLConvert(Style);
	XMLConvert(ExStyle);

	return CXElement::RegisterXMLSupportProperty();
}

XResult CXRealWnd::ProcessXMessage( CXMsg& msg )
{
	return BaseClass::ProcessXMessage(msg);
}

