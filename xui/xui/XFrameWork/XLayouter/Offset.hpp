#pragma once
#include "LayouterBase.hpp"

namespace Layouter
{
	class COffsetLayouter : public CLayouter
	{
	public:
		virtual Property::ELayoutType MyType()
		{
			return Property::ELayoutType::Offset;
		}

		virtual XResult Layout(ElementRef element)
		{
			return XResult_NotImpl;
		}
	};
}