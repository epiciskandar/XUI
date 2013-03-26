#pragma once

//#define XUI_TRACEMSG
//#define XUI_DEBUGCONSOLE

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

#pragma comment(linker, "/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='*' publicKeyToken='6595b64144ccf1df' language='*'\"")

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

	blog::CLogDeviceFile	m_fileLogger;
	blog::CLogDeviceConsole	m_consoleLogger;
};

//////////////////////////////////////////////////////////////////////////

inline CXUI::CXUI() : m_hInst(0)
{

}

inline BOOL CXUI::Initialize( HINSTANCE hInst )
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
	return TRUE;
}

inline VOID CXUI::Finalize()
{
	blog::CBLog& logger = blog::CBLog::GetInstance();
	logger.RemoveDevice(blog::BLOG_CONSOLE);
	logger.RemoveDevice(blog::BLOG_FILE);
	m_consoleLogger.Close();
	m_fileLogger.Close();
	delete &logger;
	m_atlModule.Term();
}

inline VOID CXUI::Work()
{
	MSG msg;
	while ( GetMessage(&msg, NULL, 0, 0) > 0 )
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
}

inline CXGaia& CXUI::GetGaia()
{
	return CXGaia::GetInstance();
}
