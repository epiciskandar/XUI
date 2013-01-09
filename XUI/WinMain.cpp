#include "XUI/XUI.h"

int WINAPI WinMain (HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR szCmdLine, int iCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(szCmdLine);
	UNREFERENCED_PARAMETER(iCmdShow);

	CXUI& xui = CXUI::GetInstance();
	xui.Initialize(hInstance);

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
	textRef->SetText(_T("Hello World!"));
	xui.Work();

	xui.Finalize();
	return 0;
}