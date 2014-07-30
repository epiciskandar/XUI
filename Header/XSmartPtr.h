//////////////////////////////////////////////////////////////////////////
// sample implement of refcounted base class,or as, smart pointer
// XUI Elements are all refcounted

#pragma once
#include <wtypes.h>
#include <atomic>

class IXRef
{
public:
	virtual int AddRef() = 0;
	virtual int Release() = 0;
};

#define RefcountImpl \
public: int AddRef() \
	{return ++m_refCount;} \
int Release() \
	{int ret=--m_refCount; if (ret<=0){delete this;} return ret;} \
private: \
	std::atomic<int> m_refCount = 0;

#define RefCountImplAt(_class) \
public: \
	int AddRef(){return _class::AddRef();} \
	int Release(){return _class::Release();}

//////////////////////////////////////////////////////////////////////////
// define smart pointer
// the class T, must implement interface AddRef() and Release()

template <class T>
class XSmartPtr
{
public:
	typedef T*			PointerType;
	typedef T&			RefType;

	XSmartPtr() : m_ptr(NULL){};
	XSmartPtr(const PointerType rhs) : m_ptr(rhs){if(m_ptr){m_ptr->AddRef();}};

	XSmartPtr(const XSmartPtr& rhs)
	{
		m_ptr = dynamic_cast<const PointerType>(rhs.GetPointer());
		if (m_ptr)
		{
			m_ptr->AddRef();
		}
	}

	template <typename T2>
	XSmartPtr(const XSmartPtr<T2>& rhs)
	{
		m_ptr = dynamic_cast<const PointerType>(rhs.GetPointer());
		if (m_ptr)
		{
			m_ptr->AddRef();
		}
	}
	template <typename T2>
	XSmartPtr(T2* rhs)
	{
		m_ptr = dynamic_cast<const PointerType>(rhs);
		if (m_ptr)
		{
			m_ptr->AddRef();
		}
	}
	virtual ~XSmartPtr(){if(m_ptr){m_ptr->Release();}}

	XSmartPtr operator=(const XSmartPtr& rhs)
	{
		if (m_ptr)
		{
			m_ptr->Release();
		}
		m_ptr = dynamic_cast<PointerType>(rhs.GetPointer());
		if (m_ptr)
		{
			m_ptr->AddRef();
		}
		return *this;
	}

	template <typename T2>
	XSmartPtr operator=(const XSmartPtr<T2>& rhs)
	{
		if (m_ptr)
		{
			m_ptr->Release();
		}
		m_ptr = dynamic_cast<PointerType>(rhs.GetPointer());
		if (m_ptr)
		{
			m_ptr->AddRef();
		}
		return *this;
	}
	template <typename T2>
	XSmartPtr operator=(T2* rhs)
	{
		if (m_ptr)
		{
			m_ptr->Release();
		}
		m_ptr = dynamic_cast<PointerType>(rhs);
		if (m_ptr)
		{
			m_ptr->AddRef();
		}
		return *this;
	}

	operator bool() const
	{
		return (m_ptr==NULL)? false: true;
	}
	bool operator==(const XSmartPtr& rhs)
	{
		return GetPointer() == rhs.GetPointer();
	}
	bool operator!=(const XSmartPtr& rhs)
	{
		return GetPointer() != rhs.GetPointer();
	}

	template <typename T2>
	bool operator==(const XSmartPtr<T2>& rhs)
	{
		return GetPointer() == rhs.GetPointer();
	}
	template <typename T2>
	bool operator!=(const XSmartPtr<T2>& rhs)
	{
		return GetPointer() != rhs.GetPointer();
	}
	const PointerType operator ->() const
	{
		return m_ptr;
	}
	PointerType operator ->()
	{
		return m_ptr;
	}

	PointerType* operator&()
	{
		return &m_ptr;
	}

	PointerType GetPointer() const
	{
		return m_ptr;
	}

	operator PointerType() const
	{
		return m_ptr;
	}

protected:
	T* m_ptr;
};

#define XPtr XSmartPtr