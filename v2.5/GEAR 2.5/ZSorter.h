#pragma once
#include "Logger.h"

#include <glm/glm.hpp>
#include <map>


namespace G2 
{
	class RenderSystem;

	/// This class defines...
	/// @created:	2014/06/11
	/// @author Andy Reimann <a.reimann@moorlands-grove.de>
	class ZSorter 
	{
		public:
			/// This constructs a new ZSorter.
			ZSorter(RenderSystem* renderSystem, glm::vec4 const& cameraPosition) 
				: mCameraPosition(cameraPosition),
				mRenderSystem(renderSystem) {}
			
			bool operator()(std::pair<unsigned int,int> a, std::pair<unsigned int,int> b);
			
		private:

			RenderSystem*	 mRenderSystem;
			glm::vec3		 mCameraPosition;
	};
};