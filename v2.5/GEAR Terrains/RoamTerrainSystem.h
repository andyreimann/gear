#pragma once
#include "Defines.h"

#include <G2Core/BaseSystem.h>
#include <G2/Shader.h>

#define _USE_MATH_DEFINES
#include <math.h>

namespace G2 
{
	class Frustum;
	namespace Terrain 
	{
		class RoamTerrain;


		/** This ECS-System class will take care of recalculating and rendering each RoamTerrain.
		 * @created:	2014/06/21
		 * @author Andy Reimann <a.reimann@moorlands-grove.de>
		 */
		class RoamTerrainSystem : public BaseSystem<RoamTerrainSystem,RoamTerrain>
		{
			public:
				/// This constructs a new TerrainSystem.
				TERRAINSDLL_API RoamTerrainSystem();
				
				TERRAINSDLL_API void runPhase(std::string const& name, G2::FrameInfo const& frameInfo);

			private:
				
				void _reset(RoamTerrain* terrain, G2::Frustum const* cameraFrustum);
				void _tesselate(RoamTerrain* terrain, glm::vec3 const& cameraPosition);
				void _draw(RoamTerrain* terrain);
		};
	};
};