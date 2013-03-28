#pragma once
#include "XBase.hpp"
#include "XMsg.hpp"
#include <vector>
#include <functional>

typedef std::function<XResult(CXMsg& msg)> XEar;

class CAutoEar
{
public:
	CAutoEar() : m_earID(0)
	{
		;
	}
	virtual ~CAutoEar()
	{
		;
	}
	DWORD m_earID;
};

class CXNotifier
{
	XClass(CXBase);
public:
	XResult Listen(XEar ear,DWORD& earID)
	{
		earID = m_ears.size() + 1;
		m_ears.insert(std::make_pair(earID,ear));
		return XResult_OK;
	}
	XResult GoDeaf(DWORD earID)
	{
		auto ci = m_ears.find(earID);
		if (ci != m_ears.end())
		{
			m_ears.erase(ci);
			return XResult_OK;
		}
		return XResult_NotFound;
	}
protected:
	std::map<DWORD /*id*/,XEar> m_ears;
};

MyNameIs(CXNotifier)
	I_Can("将消息发送给注册了监听的对象")
End_Description;