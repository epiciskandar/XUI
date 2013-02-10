#pragma once
#include "XBase.hpp"
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

//////////////////////////////////////////////////////////////////////////

#undef XMessage
