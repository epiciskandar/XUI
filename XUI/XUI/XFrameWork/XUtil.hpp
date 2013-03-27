#pragma once
#include "XDefine.hpp"
#include <shlobj.h>

namespace Util
{ 
	namespace Path
	{ 
		XResult GetDir(CString& path,BOOL slash=UseSlash);
		XResult GetParentDir(CString& path,BOOL slash=UseSlash);
		XResult Format(CString& path,BOOL slash=UseSlash);
		XResult FormatDirPath(CString& path,BOOL slash=UseSlash);
		XResult RemoveLastSplit(CString& path,BOOL slash=UseSlash);
		XResult GetKnownPath(REFKNOWNFOLDERID id,CString& path);
	}

	namespace String
	{
		XResult UTF8ToUnicode(LPCSTR str,CString& outStr);
		XResult UnicodeToUTF8(LPCTSTR str,CStringA& outStr);
	}
}

//////////////////////////////////////////////////////////////////////////

namespace Util
{ 
	namespace Path
	{ 
		inline XResult GetDir(CString& path,BOOL slash)
		{
			Return_OnXError(Format(path));
			int pos = path.ReverseFind(SplitChar(slash));
			if (pos == -1)
			{
				return XResult_Fail;
			}
			path = path.Left(pos);
			path += SplitChar(slash);
			return XResult_OK;
		}

		inline XResult GetParentDir(CString& path,BOOL slash)
		{
			Return_OnXError(Format(path,slash));
			Return_OnXError(RemoveLastSplit(path,slash));
			Return_OnXError(GetDir(path,slash));
			return XResult_OK;
		}

		inline XResult Format( CString& path,BOOL slash/*=TRUE*/ )
		{
			path.Trim();
			TCHAR slashes[] = {Slash,Slash,0};
			TCHAR backslashes[] = {BackSlash,BackSlash,0};
			if (slash)
			{
				path.Replace(BackSlash,Slash);
				while (path.Replace(slashes,SlashStr));
			}
			else
			{
				path.Replace(Slash,BackSlash);
				while (path.Replace(backslashes,BackSlashStr));
			}
			return XResult_OK;
		}

		inline XResult FormatDirPath( CString& path,BOOL slash/*=TRUE*/ )
		{
			Return_OnXError(Format(path,slash));
			Return_OnXError(RemoveLastSplit(path,slash));
			path += SplitChar(slash);
			return XResult_OK;
		}

		inline XResult RemoveLastSplit( CString& path,BOOL slash/*=UseSlash*/ )
		{
			while ( path.GetLength()!=0 && 
				path[path.GetLength()-1] == SplitChar(slash))
			{
				path.Delete(path.GetLength()-1,1);
			}
			return XResult_OK;
		}

		inline XResult GetKnownPath( REFKNOWNFOLDERID id,CString& path )
		{
			PWSTR answer;
			if (SUCCEEDED(SHGetKnownFolderPath(id,KF_FLAG_NO_ALIAS,0,&answer)))
			{
				path = answer;
				CoTaskMemFree(answer);
				return XResult_OK;
			}
			return XResult_Fail;
		}

	}

	namespace String
	{
		inline XResult UTF8ToUnicode( LPCSTR str,CString& outStr )
		{
			int targetLen = MultiByteToWideChar(CP_UTF8, 0, (LPCSTR)str, -1, NULL, 0);

			TCHAR* pTargetData = outStr.GetBuffer(targetLen+1);
			memset(pTargetData, 0, (targetLen+1) * sizeof(WCHAR));
			MultiByteToWideChar(CP_UTF8, 0, (LPCSTR)str, -1, (WCHAR*)pTargetData, targetLen);

			outStr.ReleaseBuffer();
			return XResult_OK;  
		}
	}
}

//////////////////////////////////////////////////////////////////////////

namespace Util
{
	namespace Class
	{
		class CRefCountImpl
		{
		public:
			ULONG STDMETHODCALLTYPE AddRef()
			{
				return InterlockedIncrement(&m_refCount);
			}
			ULONG STDMETHODCALLTYPE Release()
			{
				unsigned long ul = 0;
				if ((ul = InterlockedDecrement(&m_refCount)) == 0)
				{
					delete this;
				}
				return ul;
			}
			unsigned long m_refCount;
			CRefCountImpl():m_refCount(0){}
			virtual ~CRefCountImpl(){};
		};
	}
}