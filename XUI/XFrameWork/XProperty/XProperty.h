#pragma once
#include "XFramework/XBase.hpp"
#include "Header/IXProperty.h"
#include "XFramework/XTree.hpp"
#include "Third/CBuffer.h"

#include <map>

namespace Property
{
	class CXProperty : public IXProperty
	{
	public:
		XResult SetProperty(LPCTSTR key, const CString& value) override;
		XResult SetProperty(LPCTSTR key, const DWORD& value) override;
		XResult SetProperty(LPCTSTR key, const INT& value) override;
		XResult SetProperty(LPCTSTR key, const CRect& value) override;
		XResult SetProperty(LPCTSTR key, const CPoint& value) override;
		XResult SetProperty(LPCTSTR key, const CSize& value) override;
		XResult SetProperty(LPCTSTR key, const HWND& value) override;

		XResult GetProperty(LPCTSTR key, CString& value) const override;
		XResult GetProperty(LPCTSTR key, DWORD& value) const override;
		XResult GetProperty(LPCTSTR key, INT& value) const override;
		XResult GetProperty(LPCTSTR key, CRect& value) const override;
		XResult GetProperty(LPCTSTR key, CPoint& value) const override;
		XResult GetProperty(LPCTSTR key, CSize& value) const override;
		XResult GetProperty(LPCTSTR key, HWND& value) const override;

		XResult SetProperty(LPCTSTR key, const XNodeRef& value) override;
		XResult GetProperty(LPCTSTR key, XNodeRef& value) const override;

		bool IsChanged(LPCTSTR key) override;
		void ChangeRestore(LPCTSTR key/*empty means all*/) override;

		CXProperty& operator=(const CXProperty& rhs);
		XResult	Switch(CXProperty& rhs);
	protected:
		union ValueEnum
		{
			CString* strValue;
			DWORD dwValue;
			INT intValue;
			RECT rectValue;
			POINT ptValue;
			SIZE sizeValue;
			HWND hwndValue;
			IXNode* node;
		};
		struct Value
		{
			ValueEnum value;
			bool changed = FALSE;
			Value()
			{
				value.strValue = nullptr;
			}
		};
		std::map<CString, Value> m_propertyMap;
	};
}

