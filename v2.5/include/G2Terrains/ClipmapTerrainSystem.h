#pragma once
#include "Defines.h"
#include <G2Core/BaseSystem.h>
#include <G2/Shader.h>

namespace G2 
{
	namespace Terrain 
	{
		class ClipmapTerrain;
		/** This class defines...
		 * @created:	2014/06/21
		 * @author Andy Reimann <a.reimann@moorlands-grove.de>
		 */
		class ClipmapTerrainSystem : public BaseSystem<ClipmapTerrainSystem,ClipmapTerrain>
		{
			public:
				/// This constructs a new TerrainSystem.
				TERRAINSDLL_API ClipmapTerrainSystem();
				
				TERRAINSDLL_API void runPhase(std::string const& name, G2::FrameInfo const& frameInfo);

			private:

				void _uploadMatrices(
					std::shared_ptr<G2::Shader> shader,
					glm::mat4 const& cameraSpaceMatrix,
					glm::mat4 const& modelMatrix
				);
				/** This function will draw the clipmap ring i.
				 * @param i The number of the clipmapring to draw. Ring one starts with i=1.
				 * @param lastCamPos The converted (see function fitPosToGrid) camera position from the last ring i-1. In case of i=1, this variable is meaningless.
				 * @param camPos The converted  camera position from the ring i.
				 */
				void _drawRing(ClipmapTerrain* terrain, int i, glm::vec4 const& lastGridCamPos = glm::vec4(0.f,0.f,0.f,1.f), glm::vec4 const& camPos = glm::vec4(0.f,0.f,0.f,1.f) );
			

				
				/** This function will convert a normal point to a one which fits into the regular grid of a samplingrate of 2^(clipmapLevel-1).
				 * @pos the point to convert.
				 * @param clipmapLevel The clipmap ring starting with 1 for the first ring.
				 * @return The converted point.
				 */
				glm::vec4 fitPosToGrid(ClipmapTerrain* terrain, glm::vec4 const& pos, int clipmapLevel );

		};
	};
};