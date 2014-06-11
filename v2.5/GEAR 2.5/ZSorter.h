#pragma once
#include "RenderComponent.h"
#include "Logger.h"

#include <glm/glm.hpp>
#include <map>


namespace G2 
{
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
			
			bool operator()(std::pair<unsigned int,unsigned int> a, std::pair<unsigned int,unsigned int> b)
			{
				float distA = glm::length(mCameraPosition - mRenderSystem->get(a.first)->worldSpaceAABBs[a.second].getCenter());
				float distB = glm::length(mCameraPosition - mRenderSystem->get(b.first)->worldSpaceAABBs[b.second].getCenter());
				// objects more far away are considered to be "smaller" aka rendered earlier
				return distA > distB;
			}
			
		private:

			RenderSystem*	 mRenderSystem;
			glm::vec3		 mCameraPosition;
	};
};