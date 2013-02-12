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

			BOOL autoWidth = FALSE;
			element->GetAutoWidth(autoWidth);
			BOOL autoHeight = FALSE;
			element->GetAutoHeight(autoHeight);
			CSize elementSize;
			element->GetSize(elementSize);

			//////////////////////////////////////////////////////////////////////////
			// position children

			{
				Property::ELayoutDirection direction;
				element->GetLayoutDirection(direction);

				CRect containerRect;
				CRect lefttopRect;
				CRect middleRect;
				CRect rightbottomRect;

				NodeRef childNode;
				element->GetFirstChild(childNode);
				while(childNode)
				{
					ElementRef childElement = childNode;
					CRect rect;
					childElement->GetRect(rect);
					Property::EAlignType alignType;
					childElement->GetAlign(alignType);
					if (direction == Property::ELayoutDirection::Horizon)
					{
						if (alignType == Property::EAlignType::Top
							|| alignType == Property::EAlignType::Bottom
							|| alignType == Property::EAlignType::VCenter)
						{
							alignType = Property::EAlignType::Left;
						}
						switch (alignType)
						{
						case Property::EAlignType::Left:
							{
								CRect calculatingRect;
								calculatingRect = lefttopRect;
								CSize childSize;
								childElement->GetSize(childSize);
								calculatingRect.left = calculatingRect.right;
								calculatingRect.right = calculatingRect.left + childSize.cx;
								calculatingRect.bottom = calculatingRect.top + childSize.cy;
								calculatingRect.NormalizeRect();
								childElement->SetRect(calculatingRect);
								lefttopRect.UnionRect(lefttopRect,calculatingRect);
							}
							break;
						case Property::EAlignType::Right:
							break;
						case Property::EAlignType::Center:
						case Property::EAlignType::HCenter:
							break;
						default:
							break;
						}
					}
					else if (direction == Property::ELayoutDirection::Vertical)
					{
						if (alignType == Property::EAlignType::Left
							|| alignType == Property::EAlignType::Right
							|| alignType == Property::EAlignType::HCenter)
						{
							alignType = Property::EAlignType::Top;
						}
						switch (alignType)
						{
						case Property::EAlignType::Top:
							break;
						case Property::EAlignType::Bottom:
							break;
						case Property::EAlignType::Center:
						case Property::EAlignType::VCenter:
							break;
						default:
							break;
						}
					}
				}
			}

			return XResult_OK;
		}
	};
}