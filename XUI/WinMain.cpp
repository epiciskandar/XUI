#include "XUI/XUI.h"
#include <windows.h>

int WINAPI WinMain (HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR szCmdLine, int iCmdShow)
{
	CXFrame frame;
	frame.Load();
	frame.DoModal();
	return 0;
}