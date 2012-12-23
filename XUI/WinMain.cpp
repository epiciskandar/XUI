#include "XUI/XUI.h"

int WINAPI WinMain (HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR szCmdLine, int iCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(szCmdLine);
	UNREFERENCED_PARAMETER(iCmdShow);

	CXUI& xui = CXUI::GetInstance();
	xui.Initialize(hInstance);

	NodeRef node = xui.GetGaia().CreateChild(CXRealWnd::GetMyClassName());
	CXRealWndRef wnd = TransformNode<CXRealWndRef>(node);
	wnd->Create(NULL,NULL,_T("windowName"),WS_OVERLAPPEDWINDOW|WS_VISIBLE);
	xui.Work();

	xui.Finalize();
	return 0;
}