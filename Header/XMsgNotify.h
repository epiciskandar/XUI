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
	CXMsg_GetRealWnd()
	{
		msgDirection = MsgDirection::Up;
		msgPolicy = MsgDispatchPolicy::Processor;
	}
};

//////////////////////////////////////////////////////////////////////////
// Notify Messages

class CXMsg_FrameClick : public IXMsg
{
	XMessage(CXMsg_FrameClick);

	CPoint pt;
	BOOL ctrlState;
	BOOL shiftState;
	BOOL middleBtnState;
	BOOL rightBtnState;
	BOOL XButton1State;
	BOOL XButton2State;

	CXMsg_FrameClick()
		: ctrlState(FALSE)
		, shiftState(FALSE)
		, middleBtnState(FALSE)
		, rightBtnState(FALSE)
		, XButton1State(FALSE)
		, XButton2State(FALSE)
	{
		msgPolicy = MsgDispatchPolicy::Processor;
	}
};

class CXMsg_BeforeDeaf : public IXMsg
{
	XMessage(CXMsg_BeforeDeaf);
};

class CXMsg_RealWndClosing : public IXMsg
{
	XMessage(CXMsg_RealWndClosing);
};
