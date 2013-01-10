#pragma once
#include "XDefine.hpp"

namespace Util
{ 
	namespace Path
	{ 
		XResult GetDir(CString& path,BOOL slash=UseSlash);
		XResult GetParentDir(CString& path,BOOL slash=UseSlash);
		XResult Format(CString& path,BOOL slash=UseSlash);
		XResult FormatDirPath(CString& path,BOOL slash=UseSlash);
		XResult RemoveLastSplit(CString& path,BOOL slash=UseSlash);
	}

}

//////////////////////////////////////////////////////////////////////////

namespace Util
{ 
	namespace Path
	{ 
		XResult GetDir(CString& path,BOOL slash)
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

		XResult GetParentDir(CString& path,BOOL slash)
		{
			Return_OnXError(Format(path,slash));
			Return_OnXError(RemoveLastSplit(path,slash));
			Return_OnXError(GetDir(path,slash));
			return XResult_OK;
		}

		XResult Format( CString& path,BOOL slash/*=TRUE*/ )
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

		XResult FormatDirPath( CString& path,BOOL slash/*=TRUE*/ )
		{
			Return_OnXError(Format(path,slash));
			Return_OnXError(RemoveLastSplit(path,slash));
			path += SplitChar(slash);
			return XResult_OK;
		}

		XResult RemoveLastSplit( CString& path,BOOL slash/*=UseSlash*/ )
		{
			while ( path.GetLength()!=0 && 
				path[path.GetLength()-1] == SplitChar(slash))
			{
				path.Delete(path.GetLength()-1,1);
			}
			return XResult_OK;
		}

	}
}
