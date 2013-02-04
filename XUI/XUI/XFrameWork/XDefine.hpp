#pragma once
#include <tchar.h>

enum XResult
{
	XResult_OK = 0,
	XResult_FALSE = 1,

	XResult_Handled = 16,
	XResult_NotHandled,

	XResult_Error = 256,
	XResult_InvalidArg,
	XResult_NotImpl,
	XResult_Fail,
	XResult_NotFound,
	XResult_NotSupport,
};

enum LayoutType
{
	Layout_Offset,
	Layout_Block
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

#define RGBA(r,g,b,a)	((COLORREF)RGB(r,g,b)|a<<24)
