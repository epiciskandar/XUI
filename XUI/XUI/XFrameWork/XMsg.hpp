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

//////////////////////////////////////////////////////////////////////////

class CXMsg_Paint : public CXMsg
{
	XMessage(CXMsg_Paint);
public:
	CXDrawDevice	drawDevice;
};

//////////////////////////////////////////////////////////////////////////

#undef XMessage
