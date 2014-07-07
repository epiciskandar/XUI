#pragma once
#include "XDefine.hpp"
#include "XUtil.hpp"
#include "XSmartPtr.h"
#include "IXMsg.h"
#include "XElementDef.h"
#include "XMsgNotify.h"

class IXResPool
{
public:
	virtual XResult SetResDir(CString prefix,CString dirPath) = 0;
	virtual XResult TranslateResPath(CString& path) = 0;
};

// singleton, main

class IXUI
{
public:
	virtual BOOL Initialize(HINSTANCE hInst) = 0;
	virtual VOID Finalize() = 0;
	virtual VOID Work() = 0;
	virtual IXGaia& GetGaia() = 0;
	virtual IXResPool& GetResPool() = 0;
};

// everything start here
__declspec(dllexport) BOOL GetIXUI(IXUI** pXUI);