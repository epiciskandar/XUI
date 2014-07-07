//#include "VisualLeakDetector/include/vld.h"
#define _CRTDBG_MAP_ALLOC
#include "Header/XUI.h"
#ifdef _DEBUG
#pragma comment(lib,"../output/Lib_Debug/dll/xui.lib")
#else
#pragma comment(lib,"../output/Lib_Release/dll/xui.lib")
#endif