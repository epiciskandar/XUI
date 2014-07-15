#pragma once

class ElementUtil
{
public:
	static VOID TranslateRectToChildCoord(CRect& rect,CRect childRect)
	{
		rect.IntersectRect(rect,childRect);
		rect.OffsetRect(childRect.TopLeft());
	}

	static VOID TranslateRectToChildCoord(CRect& rect,XNodeRef child)
	{
		CRect childRect;
		ElementRef element(child);
		ATLASSERT(element);
		element->GetRect(childRect);
		TranslateRectToChildCoord(rect,childRect);
	}

	static VOID TranslatePointToChildCoord(CPoint& pt,CRect childRect)
	{
		pt.x -= childRect.left;
		pt.y -= childRect.top;
	}

	static VOID TranslatePointToChildCoord(CPoint& pt,XNodeRef child)
	{
		CRect childRect;
		ElementRef element(child);
		ATLASSERT(element);
		element->GetRect(childRect);
		TranslatePointToChildCoord(pt,childRect);
	}

	static XResult GetElementByPoint(CPoint pt,ElementRef root,ElementRef& element)
	{
		ATLASSERT(!element);
		element = nullptr;
		CRect rect;
		root->GetRect(rect);
		if (rect.PtInRect(pt))
		{
			CPoint testPoint(pt);
			TranslatePointToChildCoord(testPoint,root);
			XNodeRef childNode;
			root->GetFirstChild(childNode);
			while (childNode)
			{
				element = nullptr;
				GetElementByPoint(testPoint,childNode,element);
				if (element)
				{
					return XResult_OK;
				}
				childNode->GetRSibling(childNode);
			}
			element = root;
			return XResult_OK;
		}
		else
		{
			return XResult_NotFound;
		}
	}

	static ElementRef GetElementByHWND(ElementRef root,HWND hWndSearch)
	{
		XNodeRef child;
		root->GetFirstChild(child);
		while (child)
		{
			ATL::CWindow* pWnd = dynamic_cast<ATL::CWindow*>(child.GetPointer());
			if (pWnd)
			{
				if (pWnd->m_hWnd == hWndSearch)
				{
					return XElementRef(child);
				}
			}
			else
			{
				ElementRef finding = GetElementByHWND(child,hWndSearch);
				if (finding)
				{
					return finding;
				}
			}
			child->GetRSibling(child);
		}
		return nullptr;
	}

};
