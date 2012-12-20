#pragma once
#include "XBase.hpp"

class CXRealWnd :
	public CXBase,
	public CWindowImpl<CXRealWnd>
{
protected:
	VOID OnDestroy();
public:
	BEGIN_MSG_MAP_EX(CXRealWnd)
		MSG_WM_DESTROY(OnDestroy)
	END_MSG_MAP()
	XClass;
};

Description_Begin(CXRealWnd)
Description_End;

VOID CXRealWnd::OnDestroy()
{
	PostMessage(WM_QUIT,0,0);
}

