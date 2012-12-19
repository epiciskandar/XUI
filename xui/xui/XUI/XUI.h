#pragma once
#include <atlbase.h>

#include "XFrameWork/XDefine.hpp"
#include "XFrameWork/XFrame.hpp"
#include "XFrameWork/XWnd.hpp"
#include "XFrameWork/XRealWnd.hpp"

class CXUI
{
public:
	DISALLOW_COPY_AND_ASSIGN(CXUI);
	CXUI();
	static CXUI& GetInstance();
public:
	BOOL Initialize(HINSTANCE hInst);
	VOID Finalize();
	VOID Work();
private:
	CAppModule m_atlModule;
	HINSTANCE m_hInst;
};

//////////////////////////////////////////////////////////////////////////

CXUI::CXUI() : m_hInst(0)
{

}

CXUI& CXUI::GetInstance()
{
	static CXUI xuiMain;
	return xuiMain;
}

BOOL CXUI::Initialize( HINSTANCE hInst )
{
	ATLASSERT(m_hInst==0);
	m_hInst = hInst;
	m_atlModule.Init(NULL,m_hInst);
	return TRUE;
}

VOID CXUI::Finalize()
{
	m_atlModule.Term();
}

VOID CXUI::Work()
{
	MSG msg;
	while ( GetMessage(&msg, NULL, 0, 0) > 0 )
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
}
