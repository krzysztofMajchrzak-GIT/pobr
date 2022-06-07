#pragma once
#include <optional>
#include "SegmentInfo.h"
#include <vector>
#include "MyCv.h"
namespace MyCV
{
	class Logo
	{

	public:
		std::optional<SegmentInfo> Circle;
		std::vector<SegmentInfo> L;
		std::optional<SegmentInfo> I;
		std::optional<SegmentInfo> Dot;
		std::optional<SegmentInfo> D;
		bool isValidLogo();
		Logo();
	};
}
