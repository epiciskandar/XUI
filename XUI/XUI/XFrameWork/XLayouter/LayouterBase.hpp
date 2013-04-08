#pragma once
#include "XFrameWork/XBase.hpp"
#include "XFrameWork/XProperty.hpp"
#include "XFrameWork/XElement.h"

namespace Layouter
{
	class CLayouter : public Util::Class::CRefCountImpl
	{
		XClass;
	public:
		virtual Property::ELayoutType MyType() = 0;
		virtual XResult Layout(XSmartPtr<CXElement> element) = 0;
	};

	typedef XSmartPtr<CLayouter>	LayouterRef;
}