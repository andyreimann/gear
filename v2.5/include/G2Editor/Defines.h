#pragma once
#ifdef EDITORDLL_EXPORTS
#define EDITORDLL_API __declspec(dllexport) 
#else
#define EDITORDLL_API __declspec(dllimport) 
#endif


namespace G2
{
	namespace Editor
	{
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