#include "XUI/XUI.h"
#include "VisualLeakDetector/include/vld.h"

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
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(szCmdLine);
	UNREFERENCED_PARAMETER(iCmdShow);
	
	VLDDisable();
	CXUI& xui = CXUI::GetInstance();
	xui.Initialize(hInstance);
	VLDRestore();
	Prepare();

	CString xmlPath = _T("res:/test.xml");
	CXResPool::GetInstance().TranslateResPath(xmlPath);
	NodeRef xmlNode = xui.GetGaia().CreateFromXML(xmlPath);

	NodeRef node = xui.GetGaia().Create(CXRealWnd::GetMyClassName());
	CXRealWndRef wnd = TransformNode<CXRealWndRef>(node);
	wnd->Create(NULL,NULL,_T("windowName"),WS_OVERLAPPEDWINDOW|WS_VISIBLE);
	NodeRef textNode = xui.GetGaia().Create(CXStatic::GetMyClassName());
	CXStaticRef textRef = TransformNode<CXStaticRef>(textNode);
	node->AppendChild(textNode);
	CXRealWndRef wndRef = TransformNode<CXRealWndRef>(node);
	CRect rect;
	wndRef->GetClientRect(rect);
	textRef->SetRect(rect);
	textRef->SetText(CString(_T("Hello World!")));

	xui.Work();

	xui.Finalize();
	return 0;
}