//#include "VisualLeakDetector/include/vld.h"
#define _CRTDBG_MAP_ALLOC
#include "XUI/XUI.h"
#include "uiResponse.h"

#pragma comment(lib,"../Debug/xui.lib")

void Prepare()
{
	CString path;
	GetModuleFileName(NULL,path.GetBuffer(MAX_PATH),MAX_PATH);
	path.ReleaseBuffer();
	Util::Path::GetDir(path);
	Util::Path::GetParentDir(path);
	path += _T("Resource/");
	CXResPool::GetInstance().SetResDir(_T("res:"),path);
} 

int WINAPI WinMain (HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR szCmdLine, int iCmdShow)
{
	_CrtSetDbgFlag ( _CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF );
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(szCmdLine);
	UNREFERENCED_PARAMETER(iCmdShow);

	class CXUIListenerRegister listernerRegister;
	
	CXUI& xui = CXUI::GetInstance();
	xui.Initialize(hInstance);
	Prepare();
	xui.GetGaia().SetListenerRegister(std::bind(
		&CXUIListenerRegister::OnCreateElement,
		listernerRegister,std::placeholders::_1));

	CString xmlPath = _T("res:/test.xml");
	CXResPool::GetInstance().TranslateResPath(xmlPath);
	XNodeRef xmlNode = xui.GetGaia().CreateFromXML(xmlPath);

	CXRealWndRef wnd = xmlNode;
	if (wnd)
	{
		wnd->Create(0);
		wnd->ShowWindow(SW_SHOW);
		XNodeRef textNode;
		xmlNode->SearchChild(_T("anotherText"),textNode);
		//CXStaticRef textRef = textNode;
		//textRef->SetPosition(CPoint(100,100));
		//CXRealWndRef wndRef = xmlNode;

		xui.Work();

		wnd->RIPMySelf();
	}

	xui.Finalize();
	
	return 0;
}