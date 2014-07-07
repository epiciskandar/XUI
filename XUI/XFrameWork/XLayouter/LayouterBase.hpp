#pragma once
#include "../XBase.hpp"
#include "../XProperty.hpp"
#include "../XElement.h"

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