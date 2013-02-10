#pragma once
#include "LayouterBase.hpp"

namespace Layouter
{
	class CBlockLayouter : public CLayouter
	{
	public:
		virtual Property::ELayoutType MyType()
		{
			return Property::ELayoutType::Block;
		}

		virtual XResult Layout(ElementRef element)
		{
			Property::ELayoutType layoutType;
			element->GetLayoutType(layoutType);
			if (layoutType != MyType())
			{
				return XResult_NotSupport;
			}

			Property::EAlignType alignType;
			if (XFAILED(element->GetAlign(alignType)))
			{
				alignType = Property::AlignDefaultValue;
			}

			//////////////////////////////////////////////////////////////////////////
			//  calculate position
			CRect calculatingRect;
			
			//////////////////////////////////////////////////////////////////////////
			// calculating size
			CSize currSize;
			element->GetSize(currSize);
			calculatingRect.right = calculatingRect.left + currSize.cx;
			calculatingRect.bottom = calculatingRect.top + currSize.cy;

			element->SetRect(calculatingRect);

			return XResult_OK;
		}
	};
}