#pragma once

#include "XFrameWork/XBase.hpp"
#include "XFrameWork/XGaia.hpp"
#include "XFrameWork/XResPool.hpp"

#ifdef DEBUG
#pragma comment(lib,"debug/cximage.lib")
#pragma comment(lib,"debug/jpeg.lib")
#pragma comment(lib,"debug/jbig.lib")
#pragma comment(lib,"debug/png.lib")
#pragma comment(lib,"debug/libdcr.lib")
#pragma comment(lib,"debug/libpsd.lib")
#pragma comment(lib,"debug/jasper.lib")
#pragma comment(lib,"debug/mng.lib")
#pragma comment(lib,"debug/zlib.lib")
#else
#pragma comment(lib,"release/cximage.lib")
#pragma comment(lib,"release/jpeg.lib")
#pragma comment(lib,"release/jbig.lib")
#pragma comment(lib,"release/png.lib")
#pragma comment(lib,"release/libdcr.lib")
#pragma comment(lib,"release/libpsd.lib")
#pragma comment(lib,"release/jasper.lib")
#pragma comment(lib,"release/mng.lib")
#pragma comment(lib,"release/zlib.lib")
#endif

class CXUI
{
	XSingleton(CXUI);
public:
	BOOL Initialize(HINSTANCE hInst);
	VOID Finalize();
	VOID Work();
	CXGaia& GetGaia();
private:
	CAppModule m_atlModule;
	HINSTANCE m_hInst;
};

//////////////////////////////////////////////////////////////////////////

CXUI::CXUI() : m_hInst(0)
{

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

CXGaia& CXUI::GetGaia()
{
	return CXGaia::GetInstance();
}
