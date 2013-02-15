#pragma once
#include "XElement.hpp"

//////////////////////////////////////////////////////////////////////////

// for process WM_XX faster,we need a map
#define TranslateToXMessage(_transfunc, ...) \
	{ \
	UINT msgList[] = { __VA_ARGS__ }; \
	static std::set<UINT> handleMsgSet; \
	if (handleMsgSet.empty()) \
	for(UINT n=0; n<_countof(msgList); ++n){handleMsgSet.insert(msgList[n]);} \
	if(handleMsgSet.find(uMsg) != handleMsgSet.end()) \
		{ \
		SetMsgHandled(TRUE); \
		lResult = _transfunc(uMsg, wParam, lParam); \
		if(IsMsgHandled()) \
		return TRUE; \
		} \
	}

#define XMsgTranslater(_msg,_func) \
	case _msg: \
	{ \
		return _func(wParam,lParam); \
	} \
	break;

//////////////////////////////////////////////////////////////////////////

class CXRealWnd :
	public CXElement,
	public CWindowImpl<CXRealWnd>
{
	XClass(CXElement);
public:
	CXRealWnd();
	~CXRealWnd(){};

	BEGIN_MSG_MAP_EX(CXRealWnd)
		MSG_WM_DESTROY(OnDestroy)
		TranslateToXMessage( MessageTranslateFunc 
		, WM_PAINT
		, WM_SIZE
		)
	END_MSG_MAP();

	virtual XResult ProcessXMessage(CXMsg& msg);

	XProperty_Begin
		XProperty(Title)
		XProperty(WinStyle)
		XProperty(WinExStyle)
		XProperty(HWnd)
		XProperty(CenterWindow)
	XProperty_End;

	virtual XResult SetXMLProperty( CString name,CString value );

	XResult Create(HWND hwndParent=0);

protected:
	LRESULT MessageTranslateFunc(UINT uMsg, WPARAM wParam, LPARAM lParam);
	VOID	OnDestroy();

	LRESULT _Translate_WM_PAINT(WPARAM wParam,LPARAM lParam);
	LRESULT _Translate_WM_Size(WPARAM wParam,LPARAM lParam);

	//--------------------------------//

	VOID On_CXMsg_PropertyChanged(CXMsg_PropertyChanged& arg);
};

typedef XSmartPtr<CXRealWnd> CXRealWndRef;

MyNameIs(CXRealWnd)
	I_Provide("将windows消息转换为内部消息，并提供真窗口的一些基本维护，暴漏WTL::CWindow对外")
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
	switch(uMsg)
	{
		XMsgTranslater(WM_PAINT,_Translate_WM_PAINT);
		XMsgTranslater(WM_SIZE,_Translate_WM_Size);
	}

	return 0;
}

XResult CXRealWnd::Create( HWND hwndParent/*=0*/ )
{
	BOOL autoWidth;
	GetAutoWidth(autoWidth);
	BOOL autoHeight;
	GetAutoHeight(autoHeight);
	if ( autoWidth || autoHeight)
	{
		CXMsg_Layout msg;
		ProcessXMessage(msg);
	}
	CString title;
	GetTitle(title);
	DWORD style;
	GetWinStyle(style);
	DWORD ExStyle=0;
	GetWinExStyle(ExStyle);
	CRect rect;
	GetRect(rect);

	BOOL centerWindow;
	GetCenterWindow(centerWindow);
	if (centerWindow)
	{
		HWND hWnd = GetDesktopWindow();
		CRect desktopRect;
		::GetWindowRect(hWnd,desktopRect);
		CSize windowSize = rect.Size();
		CSize desktopSize = desktopRect.Size();
		desktopSize -= windowSize;
		if (desktopSize.cx < 0)
		{
			desktopSize.cx = 0;
		}
		if (desktopSize.cy < 0)
		{
			desktopSize.cy = 0;
		}
		rect.left = desktopSize.cx/2;
		rect.top = desktopSize.cy/2;
		rect.right = rect.left + windowSize.cx;
		rect.bottom = rect.top + windowSize.cy;
	}
	AdjustWindowRectEx(rect,style,FALSE,ExStyle);
	HWND hWnd = CWindowImpl::Create(hwndParent,rect,title,style,ExStyle);
	SetHWnd(hWnd);
	if (hWnd)
	{
		return XResult_OK;
	}
	return XResult_Fail;
}

XResult CXRealWnd::ProcessXMessage( CXMsg& msg )
{
	BaseClass::ProcessXMessage(msg);

	BEGIN_XMSG_MAP(msg)
		OnXMsg(CXMsg_PropertyChanged);
	END_XMSG_MAP;
	return XResult_OK;
}

//////////////////////////////////////////////////////////////////////////
// message translate

LRESULT CXRealWnd::_Translate_WM_PAINT( WPARAM wParam,LPARAM lParam )
{
	URP(wParam,lParam);
	CXMsg_Paint msg;
	msg.msgRet = 0;
	PAINTSTRUCT ps;
	BeginPaint(&ps);
	msg.drawDevice.invalidRect = CRect(ps.rcPaint);
	msg.drawDevice.dc = ps.hdc;

	ProcessXMessage(msg);

	BitBlt(msg.drawDevice.dc,
		msg.drawDevice.invalidRect.left,
		msg.drawDevice.invalidRect.top,
		msg.drawDevice.invalidRect.Width(),
		msg.drawDevice.invalidRect.Height(),
		msg.drawDevice.dc,
		0, 0,
		SRCCOPY
		);

	ZeroMemory(&ps,sizeof(ps));
	ps.hdc = msg.drawDevice.dc;
	ps.rcPaint = msg.drawDevice.invalidRect;
	EndPaint(&ps);
	SetMsgHandled(msg.msgHandled);
	return msg.msgRet;
}

LRESULT CXRealWnd::_Translate_WM_Size( WPARAM wParam,LPARAM lParam )
{
	if (wParam != SIZE_RESTORED)
	{
		CXMsg_SizeChanged msg;
		msg.node = this;
		msg.sizeType = (ESizeType)wParam;

		ProcessXMessage(msg);

		return msg.msgRet;
	}

	CSize size;
	size.cx = LOWORD(lParam);
	size.cy = HIWORD(lParam);
	SetSize(size);

	return 0;
}

XResult CXRealWnd::SetXMLProperty( CString name,CString value )
{
	XMLConvert_Begin(name,value)
		XMLConvert(Title)
		XMLConvert(ShowState)
		XMLConvert(WinStyle)
		XMLConvert(WinExStyle)
		XMLConvert(CenterWindow)
	XMLConvert_End

	return BaseClass::SetXMLProperty(name,value);
}

VOID CXRealWnd::On_CXMsg_PropertyChanged( CXMsg_PropertyChanged& arg )
{
	if (arg.name == Property::Rect)
	{
		HWND hWnd=0;
		GetHWnd(hWnd);
		if (hWnd)
		{
			CRect rect;
			GetRect(rect);
			SetWindowPos(0,rect,SWP_NOZORDER);
		}
	}
	else if (arg.name == Property::Size)
	{
		HWND hWnd=0;
		GetHWnd(hWnd);
		if (hWnd)
		{
			CRect clientRect;
			GetClientRect(clientRect);
			CRect windowRect;
			GetWindowRect(windowRect);
			windowRect.right -= clientRect.Width();
			windowRect.bottom -= clientRect.Height();
			
			CSize size;
			GetSize(size);
			size.cx += windowRect.Width();
			size.cy += windowRect.Height();
			SetWindowPos(0,0,0,size.cx,size.cy,SWP_NOZORDER|SWP_NOMOVE);
		}
	}
}
