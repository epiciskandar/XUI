//////////////////////////////////////////////////////////////////////////
// XUI messages,see IXGaia

#pragma once
#include <list>
#include <atlstr.h>

#define XMessage(_name) \
public: \
	static CString GetXMsgName(){return CString(L#_name);} \
	virtual CString GetMyMsgName() const {return GetXMsgName();}

#ifdef XUI_TRACEMSG
#define XMsgTrace(_msg,_id) \
	{ \
	CString fmt; \
	fmt.Format(_T("ID: %s \t Function:%s"),_id,__FUNCTIONW__); \
	_msg.processStep.push_back(fmt); \
	_msg.msgName = _msg.GetMyMsgName(); \
	}
#else
#define XMsgTrace(_msg,_id) ;
#endif

#define XMsgTraceWithID(_msg)	XMsgTrace(_msg,GetID())

// define XMsg router
#define BEGIN_XMSG_MAP(_msg) \
{ \
	CXMsg_GetListenList* pListMsg = dynamic_cast<CXMsg_GetListenList*>(&_msg);
#define OnXMsgFunc(_msg,_func) \
	if (pListMsg) \
	{ \
	pListMsg->XMsgList.push_back(_msg::GetXMsgName()); \
	} \
	else if( msg.GetMyMsgName().CompareNoCase(_msg::GetXMsgName()) == 0) \
	{ \
	_msg* pDeriMsg = dynamic_cast<_msg*>(&msg); \
	ATLASSERT(pDeriMsg && "invalid XMessage response!!!!!!!"); \
	_func(*pDeriMsg); \
	if(msg.msgHandled) \
	return XResult_Handled; \
	}
#define OnXMsg(_msg)	OnXMsgFunc(_msg,On_##_msg)
#define END_XMSG_MAP \
}

enum class MsgDispatchPolicy
{
	BroadCast,
	Processor,	// stop automatically while sb. processed
};

enum class MsgDirection
{
	Up,
	Down,
};

class IXMsg
{
	XMessage(IXMsg);
public:
	BOOL	msgHandled;
	LRESULT	msgRet;

	MsgDispatchPolicy	msgPolicy;
	MsgDirection		msgDirection;

	IXMsg()
		: msgHandled(FALSE)
		, msgRet(0)
		, msgPolicy(MsgDispatchPolicy::BroadCast)
		, msgDirection(MsgDirection::Down)
	{};

	CString msgName;
	std::list<CString>	processStep;
};