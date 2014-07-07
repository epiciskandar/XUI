//////////////////////////////////////////////////////////////////////////
// Defines,enums

#pragma once
#include "Header/XDefine.hpp"
#include <Windows.h>
#include <WindowsX.h>
#include <atlstr.h>
#include <atldef.h>
#define _WTL_NO_CSTRING
#include "Third/WTL/atlapp.h"
#include "Third/WTL/atlmisc.h"

#ifndef MAX
#define MAX(_a,_b) (_a>=_b?_a:_b)
#endif

#ifndef MIN
#define MIN(_a,_b) (_a<=_b?_a:_b)
#endif