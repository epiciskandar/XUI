#pragma once
#include "Offset.hpp"
#include "Block.hpp"

namespace Layouter
{
	static XResult GetLayouter(Property::ELayoutType type,LayouterRef& layouter)
	{
		switch (type)
		{
		case Property::ELayoutType::Offset:
			{
				static COffsetLayouter staticLayouter;
				layouter = &staticLayouter;
			}
			break;
		case Property::ELayoutType::Block:
			{
				static CBlockLayouter staticLayouter;
				layouter = &staticLayouter;
			}
			break;
		default:
			{
				return XResult_NotFound;
			}
			break;
		}
		return XResult_OK;
	}
}