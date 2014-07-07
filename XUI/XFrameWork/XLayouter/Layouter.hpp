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
				static LayouterRef keeper;
				if (!keeper)
				{
					keeper = new COffsetLayouter;
				}
				layouter = keeper;
			}
			break;
		case Property::ELayoutType::Block:
			{
				static LayouterRef keeper;
				if (!keeper)
				{
					keeper = new CBlockLayouter;
				}
				layouter = keeper;
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