#pragma once
#include "XBase.hpp"
#include "XDrawDevice.hpp"

#define XMessage(_name) \
public: \
	static CString GetXMsgName(){return CString(#_name);} \
	virtual CString GetMyMsgName() const {return GetXMsgName();}

#define WM_USER_XUIMSG	WM_USER*2+1 

class CXMsg
{
	XMessage(CXMsg);
};

class CXMsg_Paint : public CXMsg
{
	XMessage(CXMsg_Paint);
public:
	CXDrawDevice	drawDevice;
};
