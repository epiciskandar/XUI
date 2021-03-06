#include "XUI.h"
#include "VisualLeakDetector/include/vld.h"
//#define XUI_TRACEMSG
#define XUI_DEBUGCONSOLE

#include "XResPool.hpp"

#pragma comment(lib,"cximage.lib")
#pragma comment(lib,"jpeg.lib")
#pragma comment(lib,"jbig.lib")
#pragma comment(lib,"png.lib")
#pragma comment(lib,"jasper.lib")
#pragma comment(lib,"zlib.lib")

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

BOOL APIENTRY DllMain(HMODULE hModule, DWORD ul_reason_for_call, LPVOID lpReserved)
{
	URP(lpReserved);
	if (ul_reason_for_call == DLL_THREAD_ATTACH)
	{
		DisableThreadLibraryCalls(hModule);
	}
	return TRUE;
}
