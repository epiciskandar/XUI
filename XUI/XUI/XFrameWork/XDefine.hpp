#pragma once

// 只要不是0就是有错误
enum XResult
{
	XResult_OK = 0,
	XResult_Error = 1,
	XResult_InvalidArg,
	XResult_NotImpl,
	XResult_Fail,
};