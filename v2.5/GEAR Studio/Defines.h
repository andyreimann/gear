#pragma once

namespace G2
{
	namespace Studio
	{
		typedef int ComponentMask;
		namespace ComponentFlag
		{
			enum Name
			{
				CAMERA_COMPONENT = 1,
				RENDER_COMPONENT = 2,
				TRANSFORM_COMPONENT = 4,
				SPLINE_ANIMATION = 8
			};
		};
	};
};