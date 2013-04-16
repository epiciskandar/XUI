//#include "VisualLeakDetector/include/vld.h"
#define _CRTDBG_MAP_ALLOC
#ifdef _DEBUG
#include "../output/Lib_Debug/include/XUI_include.h"
#pragma comment(lib,"../output/Lib_Debug/dll/xui.lib")
#else
#include "../output/Lib_Release/include/XUI_include.h"
#pragma comment(lib,"../output/Lib_Release/dll/xui.lib")
#endif