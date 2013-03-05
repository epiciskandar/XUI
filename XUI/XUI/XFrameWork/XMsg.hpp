#pragma once
#include "XTree.hpp"
#include "XDrawDevice.hpp"

#include <list>

#define XMessage(_name) \
public: \
	static CString GetXMsgName(){return CString(#_name);} \
	virtual CString GetMyMsgName() const {return GetXMsgName();}

class CXMsg
{
	XMessage(CXMsg);
public:
	BOOL	msgHandled;
	LRESULT	msgRet;
	CXMsg() : msgHandled(FALSE),msgRet(0){};
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

class CXMsg_MouseEnter : public CXMsg
{
	XMessage(CXMsg_MouseEnter);
public:
	NodeRef prevFocusNode;
};

class CXMsg_MouseLeave : public CXMsg
{
	XMessage(CXMsg_MouseLeave);
	NodeRef newFocusNode;
};

//////////////////////////////////////////////////////////////////////////

#undef XMessage
