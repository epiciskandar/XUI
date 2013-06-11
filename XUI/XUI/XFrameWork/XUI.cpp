#include "XUI.h"
//#include "../../../VisualLeakDetector/include/vld.h"
//#define XUI_TRACEMSG
#define XUI_DEBUGCONSOLE

#include "XResPool.hpp"

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


CXUI::CXUI() : m_hInst(0)
{

}

BOOL CXUI::Initialize( HINSTANCE hInst )
{
	ATLASSERT(m_hInst==0);
	m_hInst = hInst;
	AtlInitCommonControls(ICC_ANIMATE_CLASS | 
		ICC_TREEVIEW_CLASSES | 
		ICC_STANDARD_CLASSES);
	m_atlModule.Init(NULL,m_hInst);

	blog::CBLog& logger = blog::CBLog::GetInstance();
	CString logFilePath;
	Util::Path::GetKnownPath(FOLDERID_RoamingAppData,logFilePath);
	if (!logFilePath.IsEmpty())
	{
		logFilePath += _T("/XUI");
		CreateDirectory(logFilePath,NULL);
		logFilePath += _T("/LogOut.txt");
		m_fileLogger.Open(logFilePath);
		logger.AddDevice(blog::BLOG_FILE,&m_fileLogger);
	}
#ifdef XUI_DEBUGCONSOLE
	m_consoleLogger.Open();
	logger.AddDevice(blog::BLOG_CONSOLE,&m_consoleLogger);
#endif
	logger.SetThreadName(GetCurrentThreadId(),_T("XUI Init Thread"));
	return TRUE;
}

VOID CXUI::Finalize()
{
	blog::CBLog& logger = blog::CBLog::GetInstance();
	logger.RemoveDevice(blog::BLOG_CONSOLE);
	logger.RemoveDevice(blog::BLOG_FILE);
	m_consoleLogger.Close();
	m_fileLogger.Close();
	delete &logger;
	m_atlModule.Term();
}

VOID CXUI::Work()
{
	blog::CBLog& logger = blog::CBLog::GetInstance();
	logger.SetThreadName(GetCurrentThreadId(),_T("XUI Main Thread"));

	MSG msg;
	while ( GetMessage(&msg, NULL, 0, 0) > 0 )
	{
		XLOG(_T("%d\t%d\n"),msg.hwnd,msg.message);
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
}

IXGaia& CXUI::GetGaia()
{
	return CXGaia::GetInstance();
}

IXResPool& CXUI::GetResPool()
{
	return CXResPool::GetInstance();
}

__declspec(dllexport) BOOL GetIXUI(IXUI** pXUI)
{
	if (!pXUI)
	{
		return FALSE;
	}
	*pXUI = &CXUI::GetInstance();
	return TRUE;
}

BOOL APIENTRY DllMain(HANDLE hModule, DWORD ul_reason_for_call, LPVOID lpReserved)
{
	URP(hModule,ul_reason_for_call,lpReserved);
	return TRUE;
}
