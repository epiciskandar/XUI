#pragma once
#include "LayouterBase.hpp"

namespace Layouter
{
	// helper
	class CBlockStack
	{
	public:
		VOID Push(ElementRef element,BOOL horizon)
		{
			CSize size;
			element->GetSize(size);
			if (horizon)
			{
				m_totalSize.cx += size.cx;
				m_totalSize.cy = MAX(m_totalSize.cy,size.cy);
			}
			else
			{
				m_totalSize.cy += size.cy;
				m_totalSize.cx = MAX(m_totalSize.cx,size.cx);
			}
			m_elements.push_back(element);
		}
		CSize GetSize()
		{
			return m_totalSize;
		}
		VOID Calculate(CPoint offset,Property::EAlignType alignType)
		{
			CRect rect;
			rect.TopLeft() = offset;
			rect.BottomRight() = rect.TopLeft();

			for(auto& i:m_elements)
			{
				CSize elementSize;
				i->GetSize(elementSize);
				CPoint calculatingPos;
				switch (alignType)
				{
				case Property::EAlignType::Left:
					{
						calculatingPos = rect.BottomRight();
						calculatingPos.y = rect.top;
						rect.right += elementSize.cx;
					}
					break;
				case Property::EAlignType::Top:
					break;
				case Property::EAlignType::Right:
					break;
				case Property::EAlignType::Bottom:
					break;
				default:
					break;
				}
				i->SetPosition(calculatingPos);
			}
		}
	protected:
		CSize m_totalSize;
		std::list<ElementRef> m_elements;
	};

	//////////////////////////////////////////////////////////////////////////

	class CBlockLayouter : public CLayouter
	{
	public:
		virtual Property::ELayoutType MyType()
		{
			return Property::ELayoutType::Block;
		}

		virtual XResult Layout(ElementRef element)
		{
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
			CSize elementSize;
			element->GetSize(elementSize);

			CRect childrenContainRect;

			//////////////////////////////////////////////////////////////////////////
			// position children

			{
				CBlockStack leftStack;
				CBlockStack rightStack;
				CBlockStack topStack;
				CBlockStack bottomStack;
				ElementRef centerElement;

				NodeRef childNode;
				element->GetFirstChild(childNode);

				// push into stack
				while(childNode)
				{
					ElementRef childElement = childNode;
					Property::EAlignType align = Property::EAlignType::Left;
					childElement->GetAlign(align);
					switch (align)
					{
					case Property::EAlignType::Left:
						leftStack.Push(childElement,TRUE);
						break;
					case Property::EAlignType::Top:
						topStack.Push(childElement,FALSE);
						break;
					case Property::EAlignType::Right:
						rightStack.Push(childElement,TRUE);
						break;
					case Property::EAlignType::Bottom:
						bottomStack.Push(childElement,FALSE);
						break;
					case Property::EAlignType::Center:
						{
							if (centerElement)
							{
								leftStack.Push(childElement,TRUE);
							}
							centerElement = childElement;
						}
						break;
					default:
						ATLASSERT(FALSE && "WTF!?");
						break;
					}
					childNode->GetSibling(NodeRef(),childNode);
				}

				// calculating
				CSize leftSize = leftStack.GetSize();
				CSize rightSize = rightStack.GetSize();
				CSize topSize = topStack.GetSize();
				CSize bottomSize = bottomStack.GetSize();
				CSize centerSize;
				if (centerElement)
				{
					centerElement->GetSize(centerSize);
				}
				CSize maxMiddleSize;
				maxMiddleSize.cx = MAX(MAX(topSize.cx,bottomSize.cx),centerSize.cx);
				maxMiddleSize.cy = MAX(MAX(leftSize.cy,rightSize.cy),centerSize.cy);

				CPoint leftOffset;
				leftOffset.y = topSize.cy;
				CPoint rightOffset;
				rightOffset.y = topSize.cy;
				rightOffset.x = leftSize.cx+maxMiddleSize.cx;
				CPoint topOffset;
				topOffset.x = leftSize.cx;
				CPoint bottomOffset;
				bottomOffset.x = leftSize.cx;
				bottomOffset.y = topSize.cy + maxMiddleSize.cy;

				// position
				leftStack.Calculate(leftOffset,Property::EAlignType::Left);
				rightStack.Calculate(rightOffset,Property::EAlignType::Right);
				topStack.Calculate(topOffset,Property::EAlignType::Top);
				bottomStack.Calculate(bottomOffset,Property::EAlignType::Bottom);
				if (centerElement)
				{
					centerElement->SetPosition(CPoint(leftSize.cx,topSize.cy));
				}

				if (autoWidth || autoHeight)
				{
					childrenContainRect.right = leftSize.cx + maxMiddleSize.cx + rightSize.cx;
					childrenContainRect.bottom = topSize.cy + maxMiddleSize.cy + bottomSize.cy;
				}
			}

			if (autoWidth)
			{
				elementSize.cx = childrenContainRect.Width();
			}
			if (autoHeight)
			{
				elementSize.cy = childrenContainRect.Height();
			}
			if (autoWidth || autoHeight)
			{
				element->SetSize(elementSize);
			}

			return XResult_OK;
		}
	};
}