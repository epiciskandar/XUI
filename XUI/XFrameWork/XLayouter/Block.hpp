#pragma once
#include "LayouterBase.hpp"

namespace Layouter
{
	interface IBlockPlacer
	{
	public:
		virtual ~IBlockPlacer()
		{
		}
		virtual VOID Push(ElementRef element) = 0;
		virtual CSize Place() = 0;
	};

	class CHorizonPlacer : public IBlockPlacer
	{
	public:
		CHorizonPlacer(CRect fatherRect)
			: m_fatherRect(fatherRect)
		{}
		VOID Push(ElementRef element)
		{
			Property::AlignType align;
			element->GetAlign(align);
			switch (align)
			{
			case Property::EAlignType::Left:
			case Property::EAlignType::Top:
			case Property::EAlignType::Bottom:
				m_leftElements.push_back(element);
				break;
			case Property::EAlignType::Right:
				m_rightElements.push_front(element);
				break;
			default:
				WTF;
				break;
			}
			BOOL expandwidth;
			element->GetExpandWidth(expandwidth);
			CRect rect;
			element->GetOuterLayoutRect(rect);
			if (!expandwidth)
			{
				m_smallestNeedSize.cx += rect.Width();
			}
			else
			{
				++m_expandElementCount;
			}
			m_smallestNeedSize.cy = MAX(m_smallestNeedSize.cy, rect.Height());
		}
		CSize Place()
		{
			CPoint offset(m_fatherRect.TopLeft());
			for (auto& i:m_leftElements)
			{
				_PlaceChild(i,offset);
			}
			if (m_fatherRect.Width() > m_smallestNeedSize.cx)
			{
				offset.x += m_fatherRect.Width() - m_smallestNeedSize.cx;
			}
			for (auto& i:m_rightElements)
			{
				_PlaceChild(i,offset);
			}

			m_smallestNeedSize.cx = offset.x;
			return m_smallestNeedSize;
		}
		VOID _PlaceChild(ElementRef element,CPoint& offset)
		{
			CRect rect;
			element->GetOuterLayoutRect(rect);
			rect.OffsetRect(-rect.left, -rect.top);	// 归位到(0,0)
			rect.OffsetRect(offset);	// 放置到正确位置

			BOOL expandWidth;
			element->GetExpandWidth(expandWidth);
			BOOL expandHeight;
			element->GetExpandHeight(expandHeight);
			if (expandWidth)
			{
				INT remainWidth = m_fatherRect.Width() - m_smallestNeedSize.cx;
				if (remainWidth < 0)
				{
					remainWidth = 0;
				}
				if (m_expandElementCount == 1)
				{
					rect.right = rect.left + remainWidth;
				}
				else
				{
					rect.right = rect.left + remainWidth / m_expandElementCount;
					--m_expandElementCount;
				}
				CSize size = rect.Size();
				element->CheckSizeLimit(size);	// 这里可能SizeLimit，会有大小限制
				remainWidth += rect.Width() - size.cx;
				rect.right = rect.left + size.cx;
				rect.bottom = rect.top + size.cy;	// 理论上这行应当没实际作用
			}
			if (expandHeight)
			{
				rect.bottom = rect.top + m_fatherRect.Height();
			}
			element->SetOuterLayoutRect(rect);
			offset.x += rect.Width();
		}
	protected:
		CSize	m_smallestNeedSize;
		CRect	m_fatherRect;
		std::list<ElementRef> m_leftElements;
		std::list<ElementRef> m_rightElements;
		UINT	m_expandElementCount = 0;
	};

	class CVerticalPlacer : public IBlockPlacer
	{
	public:
		CVerticalPlacer(CRect fatherRect)
			: m_fatherRect(fatherRect)
		{}
		VOID Push(ElementRef element)
		{
			Property::AlignType align;
			element->GetAlign(align);
			switch (align)
			{
			case Property::EAlignType::Left:
			case Property::EAlignType::Right:
			case Property::EAlignType::Top:
				m_topElements.push_back(element);
				break;
			case Property::EAlignType::Bottom:
				m_bottomElements.push_front(element);
				break;
			default:
				WTF;
				break;
			}
			BOOL expandHeight;
			element->GetExpandHeight(expandHeight);
			CRect rect;
			element->GetOuterLayoutRect(rect);
			if (!expandHeight)
			{
				m_smallestNeedSize.cy += rect.Height();
			}
			else
			{
				++m_expandElementCount;
			}
			m_smallestNeedSize.cx = MAX(m_smallestNeedSize.cx, rect.Width());
		}
		CSize Place()
		{
			CPoint offset(m_fatherRect.TopLeft());
			for (auto& i : m_topElements)
			{
				_PlaceChild(i, offset);
			}
			for (auto& i : m_bottomElements)
			{
				_PlaceChild(i, offset);
			}

			m_smallestNeedSize.cy = offset.y;
			return m_smallestNeedSize;
		}
		VOID _PlaceChild(ElementRef element, CPoint& offset)
		{
			CRect rect;
			element->GetOuterLayoutRect(rect);
			rect.OffsetRect(-rect.left, -rect.top);	// 归位到(0,0)
			rect.OffsetRect(offset);	// 放置到正确位置

			BOOL expandWidth;
			element->GetExpandWidth(expandWidth);
			BOOL expandHeight;
			element->GetExpandHeight(expandHeight);
			if (expandHeight)
			{
				INT remainHeight = m_fatherRect.Height() - m_smallestNeedSize.cy;
				if (remainHeight < 0)
				{
					remainHeight = 0;
				}
				if (m_expandElementCount == 1)
				{
					rect.bottom = rect.top + remainHeight;
				}
				else
				{
					rect.bottom = rect.top + remainHeight / m_expandElementCount;
					--m_expandElementCount;
				}
				CSize size = rect.Size();
				element->CheckSizeLimit(size);	// 这里可能SizeLimit，会有大小限制
				remainHeight += rect.Height() - size.cy;
				rect.right = rect.left + size.cx;	// 理论上这行应当没实际作用
				rect.bottom = rect.top + size.cy;
			}
			if (expandWidth)
			{
				rect.right = rect.left + m_fatherRect.Width();
			}
			element->SetOuterLayoutRect(rect);
			offset.y += rect.Height();
		}
	protected:
		CSize	m_smallestNeedSize;
		CRect	m_fatherRect;
		std::list<ElementRef> m_topElements;
		std::list<ElementRef> m_bottomElements;
		UINT	m_expandElementCount = 0;
	};

	//////////////////////////////////////////////////////////////////////////

	class CBlockLayouter : public CLayouter
	{
	public:
		virtual Property::ELayoutType MyType()
		{
			return Property::ELayoutType::Block;
		}

		virtual XResult Layout(XSmartPtr<CXElement> element)
		{
			CString id;
			id = element->GetID();
			Property::ELayoutType layoutType = MyType();
			element->GetLayoutType(layoutType);
			if (layoutType != MyType())
			{
				return XResult_NotSupport;
			}

			BOOL autoWidth = FALSE;
			element->GetAutoWidth(autoWidth);
			BOOL autoHeight = FALSE;
			element->GetAutoHeight(autoHeight);
			CRect elementRect;
			element->GetInnerLayoutRect(elementRect);
			if (autoWidth)
			{
				elementRect.right = elementRect.left;
			}
			if (autoHeight)
			{
				elementRect.bottom = elementRect.top;
			}
			Property::ELayoutDirection direction;
			direction = _GetElementLayoutDirection(element);

			CSize childContainSize;

			//////////////////////////////////////////////////////////////////////////
			IBlockPlacer* placer = nullptr;
			switch (direction)
			{
			case Property::ELayoutDirection::Horizon:
				placer = new CHorizonPlacer(elementRect);
				break;
			case Property::ELayoutDirection::Vertical:
				placer = new CVerticalPlacer(elementRect);
				break;
			default:
				break;
			}
			if (!placer)
			{
				WTF;
				return XResult_Error;
			}

			XNodeRef childNode;
			element->GetFirstChild(childNode);
			while (childNode)
			{
				placer->Push(childNode);
				childNode->GetRSibling(childNode);
			}

			childContainSize = placer->Place();
			delete placer;
			//////////////////////////////////////////////////////////////////////////

			if (autoWidth)
			{
				elementRect.right = elementRect.left + childContainSize.cx;
			}
			if (autoHeight)
			{
				elementRect.bottom = elementRect.top + childContainSize.cy;
			}
			if (autoWidth || autoHeight)
			{
				element->SetInnerLayoutRect(elementRect);
			}

			return XResult_OK;
		}

		Property::ELayoutDirection _GetElementLayoutDirection(ElementRef element)
		{
			Property::ELayoutDirection direction;
			if (XFAILED(element->GetLayoutDirection(direction)))
			{ // determine layout direction
				direction = Property::LayoutDirectionDefaultValue;

				XNodeRef child;
				element->GetFirstChild(child);
				ElementRef childElement(child);
				BOOL determined = FALSE;
				while (childElement)
				{
					Property::EAlignType align;
					if (XSUCCEEDED(childElement->GetAlign(align)))
					{
						switch (align)
						{
						case Property::EAlignType::Left:
						case Property::EAlignType::Right:
							direction = Property::ELayoutDirection::Horizon;
							determined = TRUE;
							break;
						case Property::EAlignType::Top:
						case Property::EAlignType::Bottom:
							direction = Property::ELayoutDirection::Vertical;
							determined = TRUE;
							break;
						}
					}
					if (determined)
					{
						element->SetLayoutDirection(direction);
						childElement = nullptr;
					}
					else
					{
						childElement->GetRSibling(child);
						childElement = child;
					}
				}
			}
			return direction;
		}
	};
}