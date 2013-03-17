#pragma once
#include "XTree.hpp"
#include "XDrawDevice.hpp"

#include <list>

#define XMessage(_name) \
public: \
	static CString GetXMsgName(){return CString(#_name);} \
	virtual CString GetMyMsgName() const {return GetXMsgName();}

#ifdef XUI_TRACEMSG
#define XMsgTrace(_msg,_id) \
	{ \
		CString fmt; \
		fmt.Format(_T("ID: %s \t Function:%s"),_id,__FUNCTIONW__); \
		_msg.processStep.push_back(fmt); \
	}
#else
#define XMsgTrace(_msg,_id) ;
#endif

#define XMsgTraceID(_msg)	XMsgTrace(_msg,GetID())

class CXMsg
{
	XMessage(CXMsg);
public:
	BOOL	msgHandled;
	LRESULT	msgRet;
	CXMsg() : msgHandled(FALSE),msgRet(0){};

#ifdef XUI_TRACEMSG
	std::list<CString>	processStep;
	~CXMsg()
	{
		if (!processStep.empty())
		{
			blog::CBLog& logger = blog::CBLog::GetInstance();
			LPCTSTR test = GetMyMsgName();
			logger.Logf(DeviceMask_All,_T("-------Msg : %s begin\n"),(LPCTSTR)GetMyMsgName());
			logger.IncreaseIndent();
			for (auto& i:processStep)
			{
				logger.Log(DeviceMask_All,(LPCTSTR)i);
			}
			logger.DecreaseIndent();
			logger.Logf(DeviceMask_All,_T("========Msg : %s end\n"),(LPCTSTR)GetMyMsgName());
		}
	}
#endif
};

//////////////////////////////////////////////////////////////////////////

class CXMsg_GetListenList : public CXMsg
{
	XMessage(CXMsg_GetListenList);
public:
	std::list<CString> XMsgList;
};

class CXMsg_PropertyChanged : public CXMsg
{
	XMessage(CXMsg_PropertyChanged);
public:
	CString name;
};

//////////////////////////////////////////////////////////////////////////

class CXMsg_Paint : public CXMsg
{
	XMessage(CXMsg_Paint);
public:
	CXDrawDevice	drawDevice;
	CPoint			offsetFix;
};

class CXMsg_Invalidate : public CXMsg
{
	XMessage(CXMsg_Invalidate);
public:
	CRect	invalidRect;
};

class CXMsg_PaintElement : public CXMsg
{
	XMessage(CXMsg_PaintElement);
public:
	BOOL	paintChildren;

	CXMsg_PaintElement():paintChildren(FALSE){}
};

class CXMsg_AttachDC : public CXMsg
{
	XMessage(CXMsg_AttachDC);
public:
	HWND hostWnd;
	HDC  hostDC;

	CXMsg_AttachDC():hostWnd(0),hostDC(0){}
};

class CXMsg_AppendElement : public CXMsg
{
	XMessage(CXMsg_AppendElement);
public:
	NodeRef element;
};

class CXMsg_SizeChanged : public CXMsg
{
	XMessage(CXMsg_SizeChanged);
public:
	NodeRef node;
	ESizeType sizeType;

	CXMsg_SizeChanged():sizeType(SizeType_Restored){}
};

class CXMsg_Layout : public CXMsg
{
	XMessage(CXMsg_Layout);
public:
};

class CXMsg_MouseMove : public CXMsg
{
	XMessage(CXMsg_MouseMove);
public:
	CPoint pt;
};

class CXMsg_MouseEnter : public CXMsg_MouseMove
{
	XMessage(CXMsg_MouseEnter);
public:
	NodeRef prevFocusNode;
};

class CXMsg_MouseLeave : public CXMsg_MouseMove
{
	XMessage(CXMsg_MouseLeave);
	NodeRef newFocusNode;
};


class CXMsg_GetHWnd : public CXMsg
{
	XMessage(CXMsg_GetHWnd);
public:
	HWND hWnd;
	CXMsg_GetHWnd() : hWnd(0){}
};

//////////////////////////////////////////////////////////////////////////

#undef XMessage
