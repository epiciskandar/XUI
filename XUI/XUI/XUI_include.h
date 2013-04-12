#pragma once
#include "XDefine.hpp"
#include "XSmartPtr.h"
#include "IXMsg.h"
#include "XElementDef.h"

class IXGaia;

class IXUI
{
public:
	virtual BOOL Initialize(HINSTANCE hInst) = 0;
	virtual VOID Finalize() = 0;
	virtual VOID Work() = 0;
	virtual IXGaia& GetGaia() = 0;
};

__declspec(dllexport) BOOL GetIXUI(IXUI** pXUI);