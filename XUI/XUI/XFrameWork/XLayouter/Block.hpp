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
		virtual CSize Place(CPoint offset) = 0;
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
			CSize size;
			element->GetSize(size);
			if (!expandwidth)
			{
				m_containSize.cx += size.cx;
			}
			m_containSize.cy = MAX(m_containSize.cy,size.cy);
		}
		CSize Place(CPoint offset)
		{
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
			element->SetPosition(offset);

			BOOL expandWidth;
			element->GetExpandWidth(expandWidth);
			BOOL expandHeight;
			element->GetExpandHeight(expandHeight);
			CSize elementSize;
			element->GetSize(elementSize);
			if (expandWidth)
			{
				elementSize.cx = m_fatherSize.cx - m_containSize.cx;
			}
			if (expandHeight)
			{
				elementSize.cy = m_fatherSize.cy;
			}
			element->SetSize(elementSize);
			offset.x += elementSize.cx;
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
			CSize size;
			element->GetSize(size);
			if (!expandHeight)
			{
				m_containSize.cy += size.cy;
			}
			m_containSize.cx = MAX(m_containSize.cx,size.cx);
		}
		CSize Place(CPoint offset)
		{
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
			element->SetPosition(offset);

			BOOL expandWidth;
			element->GetExpandWidth(expandWidth);
			BOOL expandHeight;
			element->GetExpandHeight(expandHeight);
			CSize elementSize;
			element->GetSize(elementSize);
			if (expandWidth)
			{
				elementSize.cx = m_fatherSize.cx;
			}
			if (expandHeight)
			{
				elementSize.cy = m_fatherSize.cy - m_containSize.cy;
			}
			element->SetSize(elementSize);
			offset.y += elementSize.cy;
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
			element->GetLayoutRect(elementRect);
			if (autoWidth)
			{
				elementRect.right = elementRect.left;
			}
			if (autoHeight)
			{
				elementRect.bottom = elementRect.top;
			}
			CRect padding;
			element->GetPadding(padding);
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

			childContainSize = placer->Place(padding.TopLeft());
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
				element->SetLayoutRect(elementRect);
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