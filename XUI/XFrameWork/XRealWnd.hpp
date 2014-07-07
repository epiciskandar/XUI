#pragma once
#include "XElement.h"

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

typedef CWinTraits<WS_OVERLAPPEDWINDOW | WS_CLIPCHILDREN |
	WS_CLIPSIBLINGS,
	WS_EX_APPWINDOW | WS_EX_WINDOWEDGE>
	CRealWndWinTraits;

class CXRealWnd :
	public CXElement,
	public CWindowImpl<CXRealWnd>,
	virtual public IXRealWnd
{
	XClass;
	//DECLARE_WND_CLASS_EX(_T("XUIWnd1"),CS_HREDRAW|CS_VREDRAW|CS_DROPSHADOW, COLOR_WINDOW);
public:
	CXRealWnd();
	~CXRealWnd(){};

	BEGIN_MSG_MAP_EX(CXRealWnd)
		MSG_WM_DESTROY(OnDestroy)
		TranslateToXMessage( MessageTranslateFunc 
		, WM_PAINT
		, WM_SIZE
		, WM_NCHITTEST
		, WM_MOUSEMOVE
		, WM_NCMOUSEMOVE
		, WM_LBUTTONDOWN
		, WM_LBUTTONUP
		, WM_LBUTTONDBLCLK
		, WM_CLOSE
		//, WM_CREATE
		, 0x00AE
		, 0x00AF
		, WM_CTLCOLOREDIT
		)
	END_MSG_MAP();

	virtual XResult ProcessXMessage(IXMsg& msg) override;

	XProperty(Title)
	XProperty(WinStyle)
	XProperty(WinExStyle)
	XFakeProperty_Get(HWnd)
	XProperty(CenterWindow)

	XResult Create(HWND hwndParent=0) override;

protected:
	LRESULT MessageTranslateFunc(UINT uMsg, WPARAM wParam, LPARAM lParam);
	VOID	OnDestroy();

	LRESULT _Translate_WM_PAINT(WPARAM wParam,LPARAM lParam);
	LRESULT _Translate_WM_Size(WPARAM wParam,LPARAM lParam);
	LRESULT _Translate_WM_NCHITTEST(WPARAM wParam,LPARAM lParam);
	LRESULT _Translate_WM_MOUSEMOVE(WPARAM wParam,LPARAM lParam);
	LRESULT _Translate_WM_NCMOUSEMOVE(WPARAM wParam,LPARAM lParam);
	LRESULT _Translate_MOUSEMOVE_MSG(WPARAM wParam,CPoint pointInClient);
	LRESULT _Translate_WM_CREATE(WPARAM wParam,LPARAM lParam);
	LRESULT _Translate_WM_LBUTTONDOWN(WPARAM wParam,LPARAM lParam);
	LRESULT _Translate_WM_LBUTTONUP(WPARAM wParam,LPARAM lParam);
	LRESULT _Translate_WM_LBUTTONDBLCLK(WPARAM wParam,LPARAM lParam);
	LRESULT _Translate_WM_CLOSE(WPARAM wParam,LPARAM lParam);
	LRESULT _Translate_WM_CTLCOLOREDIT(WPARAM wParam,LPARAM lParam);

	//--------------------------------//

	VOID On_CXMsg_PropertyChanged(CXMsg_PropertyChanged& arg);
	VOID On_CXMsg_AppendElement(CXMsg_AppendElement& arg);
	VOID On_CXMsg_Invalidate(CXMsg_Invalidate& arg);
	VOID On_CXMsg_GetRealWnd(CXMsg_GetRealWnd& arg);

protected:
	BOOL m_ignorePropertyChange;
	BOOL m_firstPaint;
	ElementRef	m_currFocusElement;
};

typedef XSmartPtr<CXRealWnd> CXRealWndRef;

//////////////////////////////////////////////////////////////////////////

inline CXRealWnd::CXRealWnd() : CWindowImpl()
	, m_ignorePropertyChange(FALSE)
	, m_firstPaint(TRUE)
{
}

inline VOID CXRealWnd::OnDestroy()
{
	PostMessage(WM_QUIT,0,0);
}

inline LRESULT CXRealWnd::MessageTranslateFunc( UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	switch(uMsg)
	{
		XMsgTranslater(WM_PAINT,		_Translate_WM_PAINT);
		XMsgTranslater(WM_SIZE,			_Translate_WM_Size);
		XMsgTranslater(WM_NCHITTEST,	_Translate_WM_NCHITTEST);
		XMsgTranslater(WM_MOUSEMOVE,	_Translate_WM_MOUSEMOVE);
		XMsgTranslater(WM_NCMOUSEMOVE,	_Translate_WM_NCMOUSEMOVE);
		XMsgTranslater(WM_CREATE,		_Translate_WM_CREATE);
		XMsgTranslater(WM_LBUTTONDOWN,	_Translate_WM_LBUTTONDOWN);
		XMsgTranslater(WM_LBUTTONUP,	_Translate_WM_LBUTTONUP);
		XMsgTranslater(WM_LBUTTONDBLCLK,_Translate_WM_LBUTTONDBLCLK);
		XMsgTranslater(WM_CLOSE,		_Translate_WM_CLOSE);
		XMsgTranslater(WM_CTLCOLOREDIT, _Translate_WM_CTLCOLOREDIT);
	default:
		SetMsgHandled(FALSE);
	}

	return 0;
}

inline XResult CXRealWnd::GetHWnd(Property::HWndType& value)
{
	value = m_hWnd;
	return XResult_OK;
}

inline XResult CXRealWnd::Create( HWND hwndParent/*=0*/ )
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
	HWND hWnd = CWindowImpl::Create(hwndParent,rect,title,WS_CLIPCHILDREN|WS_CLIPSIBLINGS|WS_POPUP,ExStyle);
	SetWindowLong(GWL_STYLE,style);
	SetWindowLong(GWL_EXSTYLE,ExStyle);

	if (hWnd)
	{
		CRect wndRect;
		GetWindowRect(wndRect);
		wndRect.BottomRight() -= wndRect.TopLeft();
		wndRect.TopLeft() = CPoint(0,0);
		m_ignorePropertyChange = TRUE;
		SetRect(wndRect);
		m_ignorePropertyChange = FALSE;

		return XResult_OK;
	}
	return XResult_Fail;
}

inline XResult CXRealWnd::ProcessXMessage( IXMsg& msg )
{
	__super::ProcessXMessage(msg);

	BEGIN_XMSG_MAP(msg)
		OnXMsg(CXMsg_PropertyChanged);
		OnXMsg(CXMsg_AppendElement);
		OnXMsg(CXMsg_Invalidate);
		OnXMsg(CXMsg_GetRealWnd);
	END_XMSG_MAP;
	return XResult_OK;
}

//////////////////////////////////////////////////////////////////////////
// message translate

inline LRESULT CXRealWnd::_Translate_WM_CREATE(WPARAM wParam,LPARAM lParam)
{
	URP(wParam,lParam);
	return 0;
}

inline LRESULT CXRealWnd::_Translate_WM_PAINT( WPARAM wParam,LPARAM lParam )
{
	URP(wParam,lParam);
	if (m_firstPaint)
	{
		m_firstPaint = FALSE;
		URP(wParam,lParam);
		CXMsg_AttachDC msg;
		msg.hostWnd = m_hWnd;
		msg.hostDC = GetDC();
		_SendXMsg(msg);
	}

	CXMsg_Paint msg;
	msg.msgRet = 0;
	PAINTSTRUCT ps;
	BeginPaint(&ps);
	msg.drawDevice.invalidRect = CRect(ps.rcPaint);
	msg.drawDevice.dc = ps.hdc;

	ProcessXMessage(msg);

	ZeroMemory(&ps,sizeof(ps));
	ps.hdc = msg.drawDevice.dc;
	ps.rcPaint = msg.drawDevice.invalidRect;
	EndPaint(&ps);
	SetMsgHandled(msg.msgHandled);
	return msg.msgRet;
}

inline LRESULT CXRealWnd::_Translate_WM_Size( WPARAM wParam,LPARAM lParam )
{
	if (wParam != SIZE_RESTORED)
	{
		CXMsg_SizeChanged msg;
		msg.node = this;
		msg.sizeType = (ESizeType)wParam;

		ProcessXMessage(msg);

		return msg.msgRet;
	}

	CRect rect;
	GetWindowRect(&rect);
	CSize size;
	size.cx = LOWORD(lParam);
	size.cy = HIWORD(lParam);
	m_ignorePropertyChange = TRUE;
	SetSize(size);
	m_ignorePropertyChange = FALSE;

	return 0;
}

inline VOID CXRealWnd::On_CXMsg_PropertyChanged( CXMsg_PropertyChanged& arg )
{
	if (m_ignorePropertyChange)
	{
		return;
	}

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

inline VOID CXRealWnd::On_CXMsg_AppendElement(CXMsg_AppendElement& arg)
{
	if (arg.element && m_hWnd)
	{
		CXMsg_AttachDC msg;
		msg.hostWnd = m_hWnd;
		msg.hostDC = GetDC();

		ElementRef(arg.element)->ProcessXMessage(msg);
	}
}

inline LRESULT CXRealWnd::_Translate_WM_NCHITTEST( WPARAM wParam,LPARAM lParam )
{
	URP(wParam);
	POINTS points = MAKEPOINTS(lParam);
	CPoint pt(points.x,points.y);
	ScreenToClient(&pt);
	ElementRef currHoverElement;
	ElementUtil::GetElementByPoint(pt,this,currHoverElement);
	if (currHoverElement)
	{
		DWORD dwHitTestResult;
		currHoverElement->GetHitTest(dwHitTestResult);
		return dwHitTestResult;
	}
	else
	{
		return HTNOWHERE;
	}
}

inline LRESULT CXRealWnd::_Translate_WM_MOUSEMOVE( WPARAM wParam,LPARAM lParam )
{
	CPoint pos(GET_X_LPARAM(lParam),GET_Y_LPARAM(lParam));
	return _Translate_MOUSEMOVE_MSG(wParam,pos);
}

inline LRESULT CXRealWnd::_Translate_WM_NCMOUSEMOVE( WPARAM wParam,LPARAM lParam )
{
	CPoint pos(GET_X_LPARAM(lParam),GET_Y_LPARAM(lParam));
	ScreenToClient(&pos);
	return _Translate_MOUSEMOVE_MSG(wParam,pos);
}

inline LRESULT CXRealWnd::_Translate_MOUSEMOVE_MSG( WPARAM wParam,CPoint pointInClient )
{
	URP(wParam);
	ElementRef currHoverElement;
	ElementUtil::GetElementByPoint(pointInClient,this,currHoverElement);
	if (currHoverElement)
	{
		CString id;
		id = currHoverElement->GetID();
	}
	if (currHoverElement != m_currFocusElement)
	{
		if (m_currFocusElement)
		{
			CXMsg_MouseLeave leaveMsg;
			leaveMsg.newFocusNode = currHoverElement;
			m_currFocusElement->ProcessXMessage(leaveMsg);
		}
		if (currHoverElement)
		{
			CXMsg_MouseEnter enterMsg;
			enterMsg.prevFocusNode = m_currFocusElement;
			currHoverElement->ProcessXMessage(enterMsg);
		}
		m_currFocusElement = currHoverElement;
		if (m_currFocusElement)
		{
			CXMsg_MouseMove moveMsg;
			// TODO: calculate mouse pos
			m_currFocusElement->ProcessXMessage(moveMsg);
		}
	}
	//Invalidate();
	return 0;
}

inline VOID CXRealWnd::On_CXMsg_Invalidate( CXMsg_Invalidate& arg )
{
	InvalidateRect(arg.invalidRect,FALSE);
	arg.msgHandled = TRUE;
}

inline VOID CXRealWnd::On_CXMsg_GetRealWnd( CXMsg_GetRealWnd& arg )
{
	arg.wnd = this;
	arg.msgHandled = TRUE;
}

inline LRESULT CXRealWnd::_Translate_WM_LBUTTONUP( WPARAM wParam,LPARAM lParam )
{
	CPoint pointInClient;
	pointInClient.x = GET_X_LPARAM(lParam);
	pointInClient.y = GET_Y_LPARAM(lParam);
	ElementRef currHoverElement;
	ElementUtil::GetElementByPoint(pointInClient,this,currHoverElement);
	if (currHoverElement)
	{
		CXMsg_FrameClick msg;
		msg.pt = pointInClient;
		msg.ctrlState = wParam & MK_CONTROL;
		msg.shiftState = wParam & MK_SHIFT;
		msg.middleBtnState = wParam & MK_MBUTTON;
		msg.rightBtnState = wParam & MK_RBUTTON;
		msg.XButton1State = wParam & MK_XBUTTON1;
		msg.XButton2State = wParam & MK_XBUTTON2;
		currHoverElement->ProcessXMessage(msg);
	}
	return 0;
}

inline LRESULT CXRealWnd::_Translate_WM_LBUTTONDOWN( WPARAM wParam,LPARAM lParam )
{
	URP(wParam,lParam);
	return 0;
}

inline LRESULT CXRealWnd::_Translate_WM_LBUTTONDBLCLK( WPARAM wParam,LPARAM lParam )
{
	URP(wParam,lParam);
	return 0;
}

inline LRESULT CXRealWnd::_Translate_WM_CLOSE( WPARAM wParam,LPARAM lParam )
{
	URP(wParam,lParam);
	CXMsg_RealWndClosing msg;
	ProcessXMessage(msg);
	return DefWindowProc();
}

LRESULT CXRealWnd::_Translate_WM_CTLCOLOREDIT( WPARAM wParam,LPARAM lParam )
{
	HDC hDC = (HDC)wParam;
	HWND hWnd = (HWND)lParam;
	ElementRef element = ElementUtil::GetElementByHWND(this,hWnd);
	if (element)
	{
		CXMsg_OnCtlColor msg;
		msg.hDC = hDC;
		msg.hWnd = hWnd;
		element->ProcessXMessage(msg);
		return (LRESULT)msg.hBrush;
	}
	return DefWindowProc();
}
