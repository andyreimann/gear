#pragma once
#include "Logger.h"

#include <glm/glm.hpp>
#include <map>


namespace G2 
{
	class RenderSystem;
	struct VaoIndexDrawCallPair
	{
		VaoIndexDrawCallPair()
			: vaoIndex(0),
			drawCall(0) {}
		VaoIndexDrawCallPair(unsigned int vaoIndex, unsigned int drawCall)
			: vaoIndex(vaoIndex),
			drawCall(drawCall) {}
		G2::VaoIndexDrawCallPair& operator=(G2::VaoIndexDrawCallPair const& rhs)
		{
			vaoIndex = rhs.vaoIndex;
			drawCall = rhs.drawCall;
			return *this;
		}
		unsigned int vaoIndex;
		unsigned int drawCall;
	};
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
			
			bool operator()(std::pair<unsigned int,VaoIndexDrawCallPair> a, std::pair<unsigned int,VaoIndexDrawCallPair> b);
			
		private:

			RenderSystem*	 mRenderSystem;
			glm::vec3		 mCameraPosition;
	};
};