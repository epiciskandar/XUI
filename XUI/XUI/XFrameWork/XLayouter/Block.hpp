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
		CHorizonPlacer(CSize size)
			: m_hasExpandElement(FALSE)
			, m_fatherSize(size)
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
				m_containSize.cx += rect.Width();
			}
			m_containSize.cy = MAX(m_containSize.cy,rect.Height());
		}
		CSize Place()
		{
			CPoint offset;
			for (auto& i:m_leftElements)
			{
				_PlaceChild(i,offset);
			}
			if (m_fatherSize.cx > m_containSize.cx)
			{
				offset.x += m_fatherSize.cx - m_containSize.cx;
			}
			for (auto& i:m_rightElements)
			{
				_PlaceChild(i,offset);
			}

			m_containSize.cx = offset.x;
			return m_containSize;
		}
		VOID _PlaceChild(ElementRef element,CPoint& offset)
		{
			CRect rect;
			element->GetOuterLayoutRect(rect);
			rect.TopLeft() = offset;

			BOOL expandWidth;
			element->GetExpandWidth(expandWidth);
			BOOL expandHeight;
			element->GetExpandHeight(expandHeight);
			if (expandWidth)
			{
				rect.right = rect.left + m_fatherSize.cx - m_containSize.cx;
			}
			if (expandHeight)
			{
				rect.bottom = rect.top + m_fatherSize.cy;
			}
			element->SetOuterLayoutRect(rect);
			offset.x += rect.Width();
		}
	protected:
		CSize	m_containSize;
		CSize	m_fatherSize;
		std::list<ElementRef> m_leftElements;
		std::list<ElementRef> m_rightElements;
		BOOL	m_hasExpandElement;
	};

	class CVerticalPlacer : public IBlockPlacer
	{
	public:
		CVerticalPlacer(CSize size)
			: m_hasExpandElement(FALSE)
			, m_fatherSize(size)
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
				m_containSize.cy += rect.Height();
			}
			m_containSize.cx = MAX(m_containSize.cx,rect.Width());
		}
		CSize Place()
		{
			CPoint offset;
			for (auto& i:m_topElements)
			{
				_PlaceChild(i,offset);
			}
			if (m_fatherSize.cy > m_containSize.cy)
			{
				offset.y += m_fatherSize.cy - m_containSize.cy;
			}
			for (auto& i:m_bottomElements)
			{
				_PlaceChild(i,offset);
			}

			m_containSize.cy = offset.y;
			return m_containSize;
		}
		VOID _PlaceChild(ElementRef element,CPoint& offset)
		{
			CRect rect;
			element->GetOuterLayoutRect(rect);
			rect.TopLeft() = offset;

			BOOL expandWidth;
			element->GetExpandWidth(expandWidth);
			BOOL expandHeight;
			element->GetExpandHeight(expandHeight);
			CSize elementSize;
			element->GetSize(elementSize);
			if (expandWidth)
			{
				rect.right = rect.left + m_fatherSize.cx;
			}
			if (expandHeight)
			{
				rect.bottom = rect.top + m_fatherSize.cy - m_containSize.cy;
			}
			element->SetOuterLayoutRect(rect);
			offset.y += rect.Height();
		}
	protected:
		CSize	m_containSize;
		CSize	m_fatherSize;
		std::list<ElementRef> m_topElements;
		std::list<ElementRef> m_bottomElements;
		BOOL	m_hasExpandElement;
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
				placer = new CHorizonPlacer(elementRect.Size());
				break;
			case Property::ELayoutDirection::Vertical:
				placer = new CVerticalPlacer(elementRect.Size());
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