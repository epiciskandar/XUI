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

MyNameIs(CXRealWnd)
End_Description;

//////////////////////////////////////////////////////////////////////////

VOID CXRealWnd::OnDestroy()
{
	PostMessage(WM_QUIT,0,0);
}

