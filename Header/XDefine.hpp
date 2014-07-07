//////////////////////////////////////////////////////////////////////////
// Defines,enums

#pragma once
#include <crtdefs.h>
#include <tchar.h>

#pragma comment(linker, "/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='*' publicKeyToken='6595b64144ccf1df' language='*'\"")

enum XResult
{
	XResult_OK = 0,
	XResult_FineButSomethingStrange = 1,

	XResult_Handled = 16,
	XResult_NotHandled,

	XResult_Error = 256,
	XResult_InvalidArg,
	XResult_NotImpl,
	XResult_Fail,
	XResult_NotFound,
	XResult_NotSupport,
	XResult_Forbidden,
};

enum ESizeType	// see WM_SIZE on MSDN
{
	SizeType_Restored	= 0,
	SizeType_MiniMized	= 1,
	SizeType_MaxiMized	= 2,
	SizeType_MaxShow	= 3,
	SizeType_MaxHide	= 4,
};

#define XSUCCEEDED(_result)			(_result>=XResult_Error? FALSE: TRUE)
#define XFAILED(_result)			!XSUCCEEDED(_result)
#define CheckParam(_paramsCheck)	{if(!_paramsCheck){return XResult_InvalidArg;}}
#define XUIFlag		_T("XUI")

#define UseSlash	TRUE	// 决定了库将使用哪种路径分割线作为默认值
#define Slash		_T('/')
#define SlashStr	_T("/")
#define BackSlash	_T('\\')
#define BackSlashStr	_T("\\")
#define SplitChar(_slashtype)	(_slashtype? Slash: BackSlash)

#define RGBA(r,g,b,a)	((COLORREF)RGB(r,g,b)|(a<<24))

#define WTF ATLASSERT(FALSE && "WTF!?");
#define URP(...)	(__VA_ARGS__);
#define IF_RETURN(_exp,_ret)	{if(_exp){return _ret;}}
#define Return_OnXError(_exp)	{XResult result = _exp;if(XFAILED(result)){return result;}}
