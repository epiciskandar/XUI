#pragma once

//////////////////////////////////////////////////////////////////////////
// Command Message

class CXMsg_GetListenList : public IXMsg
{
	XMessage(CXMsg_GetListenList);

	std::list<CString> XMsgList;
};
class CXMsg_GetRealWnd : public IXMsg
{
	XMessage(CXMsg_GetRealWnd);

	XPtr<IXRealWnd> wnd;
};

//////////////////////////////////////////////////////////////////////////
// Notify Messages

class CXMsg_FrameClick : public IXMsg
{
	XMessage(CXMsg_FrameClick);

	CPoint pt;
	BOOL ctrlState = FALSE;
	BOOL shiftState = FALSE;
	BOOL middleBtnState = FALSE;
	BOOL rightBtnState = FALSE;
	BOOL XButton1State = FALSE;
	BOOL XButton2State = FALSE;
};

class CXMsg_BeforeDeaf : public IXMsg
{
	XMessage(CXMsg_BeforeDeaf);
};

class CXMsg_RealWndClosing : public IXMsg
{
	XMessage(CXMsg_RealWndClosing);
};

class CXMsg_MouseAction : public IXMsg
{
	XMessage(CXMsg_MouseAction);

	enum class MouseButton
	{
		Left,
		Middle,
		Right
	};
	enum class Action
	{
		Down,
		Up,
		Click,
		DblClick
	};

	MouseButton button = MouseButton::Left;
	Action action = Action::Down;
};
