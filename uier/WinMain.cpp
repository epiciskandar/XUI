#include "stdafx.h"
#include "uiResponse.h"

void Prepare()
{
	CString path;
	GetModuleFileName(NULL,path.GetBuffer(MAX_PATH),MAX_PATH);
	path.ReleaseBuffer();
	Util::Path::GetDir(path);
	Util::Path::GetParentDir(path);
	path += _T("Resource/");
	IXUI* pXUI = nullptr;
	GetIXUI(&pXUI);
	pXUI->GetResPool().SetResDir(_T("res:"),path);
} 

int WINAPI WinMain (HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR szCmdLine, int iCmdShow)
{
	_CrtSetDbgFlag ( _CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF );
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(szCmdLine);
	UNREFERENCED_PARAMETER(iCmdShow);

	class CXUIListenerRegister listernerRegister;

	IXUI* pXUI = nullptr;
	GetIXUI(&pXUI);
	pXUI->Initialize(hInstance);
	Prepare();
	pXUI->GetGaia().SetListenerRegister(std::bind(
		&CXUIListenerRegister::OnCreateElement,
		listernerRegister,std::placeholders::_1));

	CString xmlPath = _T("res:/test.xml");
	pXUI->GetResPool().TranslateResPath(xmlPath);
	XNodeRef xmlNode = pXUI->GetGaia().CreateFromXML(xmlPath);

	XRealWndRef wnd = xmlNode;
	if (wnd)
	{
		wnd->Create();
		HWND hWnd = 0;
		wnd->GetHWnd(hWnd);
		ShowWindow(hWnd,SW_SHOW);

		pXUI->Work();

		XNodeRef(wnd)->RIPMySelf();
	}

	pXUI->Finalize();
	
	return 0;
}