#pragma once
#include "XElement.hpp"

class CXRealWnd :
	public CXElement,
	public CWindowImpl<CXRealWnd>
{
	XClass;
public:
	BEGIN_MSG_MAP_EX(CXRealWnd)
		MSG_WM_DESTROY(OnDestroy)
		TranslateToXMessage( MessageTranslateFunc 
		,WM_PAINT )
	END_MSG_MAP();

	BEGIN_XMESSAGE_MAP
	END_XMESSAGE_MAP;
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

