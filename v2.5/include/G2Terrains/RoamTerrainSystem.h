#pragma once
#include "Defines.h"

#include <G2Core/BaseSystem.h>
#include <G2/Shader.h>

#define _USE_MATH_DEFINES
#include <math.h>

namespace G2 
{
	namespace Terrain 
	{
		class RoamTerrain;


		/** This class defines...
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
				
				void _reset(RoamTerrain* terrain, glm::vec3 const& cameraPosition, float fovY, float cameraYawAngle);
				void _tesselate(RoamTerrain* terrain, glm::vec3 const& cameraPosition);
				void _draw(RoamTerrain* terrain, std::shared_ptr<G2::Shader> shader, glm::mat4 const& cameraSpaceMatrix, glm::mat4 const& modelMatrix);

				/** This function will upload all the matrices to the shader used for rendering the terrain
				 * @param shader The shader to upload the matrices to (already bound).
				 * @param cameraSpaceMatrix The camera space matrix to upload.
				 * @param modelMatrix The model space matrix to upload.
				 */
				void _uploadMatrices(
					std::shared_ptr<G2::Shader> shader,
					glm::mat4 const& cameraSpaceMatrix,
					glm::mat4 const& modelMatrix
				);
		};
	};
};