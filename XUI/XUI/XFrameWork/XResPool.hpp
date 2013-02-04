#pragma once
#include "XBase.hpp"

#include <map>

class CXResPool
{
	XClass(VOID);
	XSingleton(CXResPool);
public:
	XResult SetResDir(CString prefix,CString dirPath);
	XResult TranslateResPath(CString& path);
protected:
	std::map<CString/*prefix*/,CString/*path*/>	m_convertMap;
};

MyNameIs(CXResPool)
	I_Provide("资源目录的管理及路径字符串的转换")
End_Description;

CXResPool::CXResPool()
{}

XResult CXResPool::SetResDir( CString prefix,CString dirPath )
{
	m_convertMap[prefix] = dirPath;
	return XResult_OK;
}

XResult CXResPool::TranslateResPath( CString& path )
{
	for (auto i=m_convertMap.begin(); i!=m_convertMap.end(); ++i)
	{
		if ( path.Replace(i->first,i->second) != 0 )
		{
			return XResult_OK;
		}
	}
	return XResult_Fail;
}
