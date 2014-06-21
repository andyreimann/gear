#include "ClipmapTerrainSystem.h"
#include "ClipmapTerrain.h"

#include <G2Core/ECSManager.h>
#include <G2/RenderComponent.h>
#include <G2/CameraComponent.h>
#include <G2/TransformComponent.h>
#include <utility>

using namespace G2::Terrain;

ClipmapTerrainSystem::ClipmapTerrainSystem() 
{
}

void
ClipmapTerrainSystem::runPhase(std::string const& name, G2::FrameInfo const& frameInfo) 
{
	if(name == "render")
	{
		// camera is updated, prepare for rendering of terrain

		auto* transformSystem = ECSManager::getShared().getSystem<TransformSystem,TransformComponent>();
		auto* cameraSystem = ECSManager::getShared().getSystem<CameraSystem,CameraComponent>();
		auto* renderSystem = ECSManager::getShared().getSystem<G2::RenderSystem,G2::RenderComponent>();
		
		CameraComponent* camera = cameraSystem->getRenderCamera();
		assert(camera != nullptr);
		
		glm::mat4 cameraSpaceMatrix;
		auto* cameraTransformation = transformSystem->get(camera->getEntityId());
		glm::mat4 inverseCameraRotation;
		glm::vec3 cameraPosition;
		if(cameraTransformation != nullptr)
		{
			cameraSpaceMatrix = cameraTransformation->getWorldSpaceMatrix();
			inverseCameraRotation = camera->getInverseCameraRotation();
			cameraPosition = cameraTransformation->getPosition();
		}


		
		auto& defaultEffect = renderSystem->getDefaultEffect();
		auto& shader = defaultEffect->getShader();
		shader->bind();
		for(auto i = 0; i < components.size(); ++i) 
		{
			ClipmapTerrain& terrain = components[i];
			auto shader = terrain.mEffect->getShader();
			shader->bind();
			terrain.mHeightMap->bind(G2::TEX_SLOT1 + G2::Sampler::SAMPLER_6);
			terrain.mVertexArray.bind();

			shader->setProperty(std::string("matrices.projection_matrix"), camera->getProjectionMatrix());

			glm::vec4 camPos(0.f,0.f,0.f,1.f);
			
			camPos = glm::inverse(cameraSpaceMatrix) * camPos;
			// adjust the cameras position to be centered in every terrain tile of the finest level
			camPos -= glm::vec4( 0.5f * terrain.mTileSize, 0.0f, 0.5f * terrain.mTileSize, 0.f);

			glm::vec4 gridCamPos;
			gridCamPos = fitPosToGrid( &terrain, camPos, 1 ); // make it move in next coarse grid 
			shader->setProperty( "_camX", gridCamPos.x );
			shader->setProperty( "_camZ", gridCamPos.z );
			shader->setProperty( "_maxHeight", 0.3f );
			shader->setProperty( "_heightmapWidth", (float)terrain.mHeightMap->getWidth());
			shader->setProperty( "_tileSize", (float)terrain.mTileSize);
		
			
			shader->setProperty("material.ambient", glm::vec4(0.1f,0.3f,0.1f,1.f));
			_uploadMatrices(shader, cameraSpaceMatrix, glm::translate(terrain.mClipmapWidth * terrain.mTileSize, 0.0f, terrain.mClipmapWidth * terrain.mTileSize));
			shader->setProperty( "_texScale", 1.0f );
			shader->setProperty( "_texCoordOffset", - glm::vec2(((terrain.mClipmapWidth+1.0f))/(float)terrain.mHeightMap->getWidth(),((terrain.mClipmapWidth+1.0f))/(float)terrain.mHeightMap->getHeight()) );

			glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
			GLDEBUG( glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, terrain.mViewport.id ) );
			GLDEBUG( glDrawElements( GL_TRIANGLE_STRIP, terrain.mViewport.numIndices, GL_UNSIGNED_INT, NULL) );
			GLDEBUG( glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, NULL) );

			/*
			shader->setProperty("material.ambient", glm::vec4(0.3f,0.1f,0.1f,1.f));
			GLDEBUG( glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, terrain.mMMInstanceBlock.id ) );
			GLDEBUG( glDrawElements( GL_TRIANGLE_STRIP, terrain.mMMInstanceBlock.numIndices, GL_UNSIGNED_INT, NULL) );
			GLDEBUG( glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, NULL) );
			*/

			int numRings = 1;
			glm::vec4 lastGridCamPos;
			for(int i = 1; i <= numRings; ++i)
			{
				gridCamPos = fitPosToGrid( &terrain, camPos, i ); // make it move in next coarse grid 
				float mult = 1.f / std::pow( 2.f, (float)(i-1));

				
				shader->setProperty( "_texScale", std::pow( 2.0f, (float)(i-1)) );
				shader->setProperty( "_camX", gridCamPos.x*mult ); // set scaled camera position
				shader->setProperty( "_camZ", gridCamPos.z*mult ); // set scaled camera position

				_drawRing( &terrain, i, lastGridCamPos, gridCamPos );
			}

			terrain.mVertexArray.unbind();

		}
	}
}

void
ClipmapTerrainSystem::_uploadMatrices(std::shared_ptr<G2::Shader> shader, glm::mat4 const& cameraSpaceMatrix, glm::mat4 const& modelMatrix) 
{
	{
		shader->setProperty(std::string("matrices.model_matrix"), modelMatrix);
		shader->setProperty(std::string("matrices.view_matrix"), cameraSpaceMatrix);
		shader->setProperty(std::string("matrices.modelview_matrix"), cameraSpaceMatrix * modelMatrix);
		shader->setProperty(std::string("matrices.normal_matrix"), glm::mat3(cameraSpaceMatrix));
	}
}

glm::vec4 
ClipmapTerrainSystem::fitPosToGrid(ClipmapTerrain* terrain, glm::vec4 const& pos, int clipmapLevel )
{
	float mult = std::pow(2.0f, (float)clipmapLevel);
	glm::vec4 p = pos;
	if( clipmapLevel%2 == 0)
		p += glm::vec4( -terrain->_getJacobsthalNumber(clipmapLevel+1) * terrain->mTileSize, 0.0f, -terrain->_getJacobsthalNumber(clipmapLevel+1) * terrain->mTileSize, 0.f);
	else if( clipmapLevel%2 == 1) 
		p += glm::vec4( -terrain->_getJacobsthalNumber(clipmapLevel+2) * terrain->mTileSize, 0.0f, -terrain->_getJacobsthalNumber(clipmapLevel+2) * terrain->mTileSize, 0.f);
	float dx = p.x - terrain->mStartPosition.x;
	float dz = p.z - terrain->mStartPosition.z;
	float tx = (dx / (terrain->mTileSize*mult));
	float tz = (dz / (terrain->mTileSize*mult));

	glm::vec4 ret = p;
	if( tx > 0.0f )
		++tx;
	if( tz > 0.0f )
		++tz;
	ret.x = terrain->mStartPosition.x + ((int)tx * terrain->mTileSize*mult);
	ret.z = terrain->mStartPosition.z + ((int)tz * terrain->mTileSize*mult);
	return ret;
}

void
ClipmapTerrainSystem::_drawRing(ClipmapTerrain* terrain, int i, glm::vec4 const& lastGridCamPos /*= glm::vec4(0.f,0.f,0.f,1.f)*/, glm::vec4 const& camPos /*= glm::vec4(0.f,0.f,0.f,1.f) */) 
{

}