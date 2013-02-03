//#include "VisualLeakDetector/include/vld.h"
#define _CRTDBG_MAP_ALLOC
#include "XUI/XUI.h"

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
	
	CXUI& xui = CXUI::GetInstance();
	xui.Initialize(hInstance);
	Prepare();

	CString xmlPath = _T("res:/test.xml");
	CXResPool::GetInstance().TranslateResPath(xmlPath);
	NodeRef xmlNode = xui.GetGaia().CreateFromXML(xmlPath);

	//NodeRef node = xui.GetGaia().Create(CXRealWnd::GetMyClassName());
	CXRealWndRef wnd = TransformNode<CXRealWndRef>(xmlNode);
	wnd->Create(NULL,NULL,_T("windowName"),WS_OVERLAPPEDWINDOW|WS_VISIBLE);
	NodeRef textNode;
	xmlNode->GetFirstChild(textNode);
	CXStaticRef textRef = TransformNode<CXStaticRef>(textNode);
	//node->AppendChild(textNode);
	CXRealWndRef wndRef = TransformNode<CXRealWndRef>(xmlNode);
	CRect rect;
	wndRef->GetClientRect(rect);
	textRef->SetRect(rect);
	//textRef->SetText(CString(_T("Hello World!")));

	xui.Work();

	xui.Finalize();
	
	return 0;
}