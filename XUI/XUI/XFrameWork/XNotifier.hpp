#pragma once
#include "XBase.hpp"
#include "XMsg.hpp"
#include <vector>
#include <functional>

typedef std::function<XResult(CXMsg& msg)> XEar;

class CAutoEar
{
public:
	CAutoEar()
	{
		;
	}
	virtual ~CAutoEar()
	{
		;
	}
};

class CXNotifier : public CXBase
{
	XClass(CXBase);
public:
	XResult Listen(XEar ear)
	{
		m_ears.push_back(ear);
		return XResult_OK;
	}
	XResult GoDeaf(XEar ear)
	{
		auto ci = std::find(m_ears.begin(),m_ears.end(),ear);
		if (ci != m_ears.end())
		{
			m_ears.erase(ci);
			return XResult_OK;
		}
		return XResult_NotFound;
	}
protected:
	std::vector<XEar> m_ears;
};

MyNameIs(CXNotifier)
	I_Can("将消息发送给注册了监听的对象")
End_Description;