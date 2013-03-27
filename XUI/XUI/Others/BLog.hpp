#pragma once
#include "vector"
#include "map"
#include "time.h"
#include "atlstr.h"
#include "atlsync.h"
#include <atltime.h>

namespace blog
{

	// 除了标准类型以外想加入新输出端(如多file输出)，自行定义一个数即可，不需要在这里添加定义，用static_cast强转类型
	enum BLOG_TYPE
	{
		BLOG_NOLOG = 0,
		BLOG_DBGVIEW = 1,
		BLOG_FILE = 2,
		BLOG_CONSOLE = 4
	};

#define DeviceMask_All	0xffffffff

#define TIME_FORMAT_STRING _T("%Y-%m-%d %H:%M:%S")

	class CLogDeviceBase
	{
	public:
		virtual ~CLogDeviceBase(){};
		virtual BOOL Open(LPCTSTR param=NULL) = 0;
		virtual void Close() = 0;
		virtual void Write(LPCTSTR strLog) = 0;
		virtual void flush() = 0;
	};

	typedef std::map<DWORD,CLogDeviceBase*> LogDeviceListType;
	typedef std::vector<CString>			PrefixVectorType;
	typedef std::map<DWORD,CString>			ThreadNameMap;

	class CBLog
	{
	public:
		CBLog(void);
		~CBLog(void);
		static CBLog& GetInstance()
		{
			static CBLog* pInstance = nullptr;
			if (!pInstance)
			{
				pInstance = new CBLog;
				pInstance->m_instance = pInstance;
			}
			return *pInstance;
		}
	public:
		void SetThreadName(DWORD threadID,LPCTSTR name);

		void Log(DWORD deviceMask,LPCTSTR strlog,BOOL info=TRUE,BOOL autoEndLine=TRUE);
		void Logf(DWORD deviceMask,LPCTSTR fmt,...);

		void AddDevice(BLOG_TYPE type,CLogDeviceBase* device);
		CLogDeviceBase* GetDevice(BLOG_TYPE type);
		CLogDeviceBase* RemoveDevice(BLOG_TYPE type);

		void AddPrefix(LPCTSTR strPrefix);
		void RemoveLastPrefix();

		void IncreaseIndent(){++m_indent;};
		void DecreaseIndent(){if(m_indent)--m_indent;};

	private:
		void _AddInfo(CString &strLog);
		void _AddPrefix(CString &strLog);
		void _AddIndent(CString &strlog);
	private:
		DWORD m_indent;
		LogDeviceListType m_logDevice;
		PrefixVectorType m_Prefix;
		ThreadNameMap m_threadNameMap;
		ATL::CCriticalSection	m_cs;
		CBLog* m_instance;
	};

	//////////////////////////////////////////////////////////////////////////
	// 
	inline CBLog::CBLog(void) : m_instance(0), m_indent(0)
	{
	}

	inline CBLog::~CBLog(void)
	{
		LogDeviceListType::iterator i;
		i = m_logDevice.begin();
		while (i != m_logDevice.end())
		{
			i->second->Close();
			delete i->second;
			++i;
		}
	}

	inline void CBLog::AddDevice( BLOG_TYPE type,CLogDeviceBase* device )
	{
		m_cs.Enter();
		if (m_logDevice.find(type) != m_logDevice.end())
		{
			delete m_logDevice[type];
		}
		m_logDevice.insert(std::make_pair(type,device));
		m_cs.Leave();
	}

	inline void CBLog::Log( DWORD deviceMask,LPCTSTR strlog,BOOL info/*=TRUE*/,BOOL autoEndLine/*=TRUE*/ )
	{
		CString logText;
		if (info)
		{
			_AddInfo(logText);
			_AddPrefix(logText);
			logText += _T('|');
			logText += _T('\t');
			_AddIndent(logText);
		}
		logText += strlog;
		if (autoEndLine)
		{
			logText += _T('\n');
		}

		m_cs.Enter();
		LogDeviceListType::iterator i;
		i = m_logDevice.begin();
		while (i != m_logDevice.end())
		{
			if (deviceMask & i->first)
			{
				i->second->Write(logText);
			}
			++i;
		}
		m_cs.Leave();
	}

	inline void CBLog::Logf( DWORD deviceMask,LPCTSTR fmt,... )
	{
		CString strFormat;
		va_list vlist;
		va_start(vlist,fmt);
		strFormat.FormatV(fmt,vlist);
		va_end(vlist);
		return Log(deviceMask,strFormat,TRUE,FALSE);
	}

	inline void CBLog::_AddIndent(CString &strLog)
	{
		if (m_indent)
		{
			strLog += _T(" ");
		}
		for (UINT n=0; n<m_indent; ++n)
		{
			strLog += _T('\t');
		}
	}

	inline void CBLog::_AddInfo( CString &strLog )
	{
		time_t ct = time(NULL);
		tm otm;
		localtime_s(&otm, &ct);
		SYSTEMTIME st;
		GetSystemTime(&st);
		strLog.Format(_T("%d-%d-%d %d:%d:%d.%d \t"),
			otm.tm_year+1900,otm.tm_mon+1,otm.tm_mday,
			otm.tm_hour,otm.tm_min,otm.tm_sec,st.wMilliseconds
			);
		DWORD threadID = GetCurrentThreadId();
		ThreadNameMap::iterator i;
		i = m_threadNameMap.find(threadID);
		if ( i != m_threadNameMap.end() )
		{
			strLog.AppendFormat(_T("%s(%d) \t"), i->second,threadID);
		}
		else
		{
			strLog.AppendFormat(_T("%d \t"), threadID);
		}
	}

	inline void CBLog::_AddPrefix( CString &strLog )
	{
		PrefixVectorType::iterator i;
		i = m_Prefix.begin();
		if (i == m_Prefix.end())
		{
			return;
		}
		while (i != m_Prefix.end())
		{
			strLog += *i++;
			if (i != m_Prefix.end())
			{
				strLog +=_T("::");
			}
		}
		//strLog += _T(" \t");
	}

	inline void CBLog::AddPrefix( LPCTSTR strPrefix )
	{
		ATLASSERT(strPrefix);
		if (strPrefix)
		{
			m_Prefix.push_back(strPrefix);
		}
	}

	inline void CBLog::RemoveLastPrefix()
	{
		if (!m_Prefix.empty())
		{
			m_Prefix.pop_back();
		}
	}

	inline CLogDeviceBase* CBLog::GetDevice( BLOG_TYPE type )
	{
		LogDeviceListType::iterator i;
		i = m_logDevice.find(type);
		if (i != m_logDevice.end())
		{
			return i->second;
		}
		return NULL;
	}

	inline CLogDeviceBase* CBLog::RemoveDevice( BLOG_TYPE type )
	{
		LogDeviceListType::iterator i;
		i = m_logDevice.find(type);
		if ( i != m_logDevice.end())
		{
			CLogDeviceBase* pBase = i->second;
			m_logDevice.erase(i);
			return pBase;
		}
		return NULL;
	}

	inline void CBLog::SetThreadName( DWORD threadID,LPCTSTR name )
	{
		ThreadNameMap::iterator i;
		i = m_threadNameMap.find(threadID);
		if ( i != m_threadNameMap.end())
		{
			i->second = name;
		}
		else
		{
			m_threadNameMap.insert(std::make_pair(threadID,name));
		}
	}


	//////////////////////////////////////////////////////////////////////////

	class CLogDeviceFile:public CLogDeviceBase
	{
	public:
		virtual BOOL Open(LPCTSTR param=NULL)
		{
			m_fp = NULL;
			CString logFile;
			if (param)
			{
				logFile = param;
				errno_t err = _tfopen_s(&m_fp,logFile,_T("at+"));
				err = err;
				if (m_fp)
				{
					CTime currTime = CTime::GetCurrentTime();
					_ftprintf_s(m_fp,_T("-----------------------------------\nLog Generated At %s\n"),currTime.Format(TIME_FORMAT_STRING));
				}
			}
			return m_fp!=NULL;
		}
		virtual void Close()
		{
			if (m_fp)
			{
				fclose(m_fp); m_fp = NULL;
			}
		}
		virtual void Write(LPCTSTR strLog)
		{
			if (strLog && m_fp)
			{
				_ftprintf_s(m_fp,_T("%s"),strLog);
			}
		}
		virtual void flush()
		{
			if (m_fp)
			{
				fflush(m_fp);
			}
		}
	private:
		CString m_file;
		FILE *m_fp;
	};

	class CLogDeviceDBGView:public CLogDeviceBase
	{
	public:
		virtual BOOL Open(LPCTSTR param=NULL){param;return TRUE;};
		virtual void Close(){};
		virtual void Write(LPCTSTR strLog)
		{
			::OutputDebugString(strLog);
		};
		virtual void flush(){};
	};

	class CLogDeviceMem : public CLogDeviceBase
	{
	public:
		virtual BOOL Open(LPCTSTR param=NULL){param;return TRUE;};
		virtual void Close(){};
		virtual void Write(LPCTSTR strLog)
		{
			m_log += strLog;
		};
		virtual void flush(){};

		void get_log(std::wstring& str){str = CStringW(m_log);};

	private:
		CString m_log;
	};


	class CLogDeviceConsole : public CLogDeviceBase
	{
	public:
		CLogDeviceConsole() : m_free_console_on_close(false)
			, m_handle(NULL)
			, m_default_attr(0)
		{

		}

		virtual BOOL Open(LPCTSTR param=NULL)
		{
			param;
			if (::AllocConsole())
			{
				m_free_console_on_close = true;
			}
			m_handle = ::GetStdHandle(STD_OUTPUT_HANDLE);
			if (is_handle_valid(m_handle))
			{
				CONSOLE_SCREEN_BUFFER_INFO csbi;
				GetConsoleScreenBufferInfo(m_handle, &csbi);
				m_default_attr = csbi.wAttributes;
				return TRUE;
			}
			return FALSE;
		}
		virtual void Close()
		{
			if (m_free_console_on_close)
			{
				::FreeConsole();
				m_handle = NULL;
			}
		}
		virtual void flush(){}

		virtual void Write(LPCTSTR strLog)
		{
			if (!is_handle_valid(m_handle)) return;
			DWORD wrote;
			::WriteConsole(m_handle, strLog, static_cast<DWORD>(_tcslen(strLog)), &wrote, NULL);
		}

	protected:
		HANDLE m_handle;
		WORD m_default_attr;
		bool m_free_console_on_close;
		bool padding[1];

	private:
		static bool is_handle_valid(HANDLE handle)
		{
			return (handle != NULL && handle != INVALID_HANDLE_VALUE);
		}
	};
}

namespace blog
{
	class CAutoBLogIndent
	{
	public:
		CAutoBLogIndent(CBLog *theLog) : m_theLog(NULL)
		{
			if (theLog)
			{
				m_theLog = theLog;
				theLog->IncreaseIndent();
			}
		};
		~CAutoBLogIndent()
		{
			if (m_theLog)
			{
				m_theLog->DecreaseIndent();
			}
		}
		CBLog *m_theLog;
	};

	class CAutoBLogPrefix
	{
	public:
		CAutoBLogPrefix(CBLog *theLog,LPCTSTR text){if(theLog){m_theLog=theLog;theLog->AddPrefix(text);}}
		~CAutoBLogPrefix(){m_theLog->RemoveLastPrefix();}
		CBLog *m_theLog;
	};
}
