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
			shader->setProperty( "_maxHeight", 5.f );
			shader->setProperty( "_heightmapWidth", (float)terrain.mHeightMap->getWidth());
			shader->setProperty( "_tileSize", (float)terrain.mTileSize);
		
			
			shader->setProperty("material.ambient", glm::vec4(1.f,1.f,0.1f,1.f));
			_uploadMatrices(shader, cameraSpaceMatrix, glm::translate(terrain.mClipmapWidth * terrain.mTileSize, 0.0f, terrain.mClipmapWidth * terrain.mTileSize));
			shader->setProperty( "_texScale", 1.0f );
			glm::vec2 texCoordOffset = -glm::vec2(((terrain.mClipmapWidth+1.f))/(float)terrain.mHeightMap->getWidth(),((terrain.mClipmapWidth+1.f))/(float)terrain.mHeightMap->getHeight());
			shader->setProperty( "_texCoordOffset", texCoordOffset );

			glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
			GLDEBUG( glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, terrain.mViewport.id ) );
			GLDEBUG( glDrawElements( GL_TRIANGLE_STRIP, terrain.mViewport.numIndices, GL_UNSIGNED_INT, NULL) );
			GLDEBUG( glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, NULL) );

			int numRings = 10;
			glm::vec4 lastGridCamPos;
			for(int i = 1; i <= numRings; ++i)
			{
				gridCamPos = fitPosToGrid( &terrain, camPos, i ); // make it move in next coarse grid 
				float mult = 1.f / std::pow( 2.f, (float)(i-1));

				shader->setProperty( "_texScale", std::pow( 2.0f, (float)(i-1)) );
				shader->setProperty( "_camX", gridCamPos.x*mult ); // set scaled camera position
				shader->setProperty( "_camZ", gridCamPos.z*mult ); // set scaled camera position
				
				_drawRing( &terrain, i, numRings, shader, cameraSpaceMatrix, lastGridCamPos, gridCamPos );
				// set this cam pos as the last one after draw call
				lastGridCamPos = gridCamPos;
			}

			terrain.mVertexArray.unbind();

		}
	}
}

void
ClipmapTerrainSystem::_uploadMatrices(std::shared_ptr<G2::Shader> shader, glm::mat4 const& cameraSpaceMatrix, glm::mat4 const& modelMatrix) 
{
	shader->setProperty(std::string("matrices.model_matrix"), modelMatrix);
	shader->setProperty(std::string("matrices.view_matrix"), cameraSpaceMatrix);
	shader->setProperty(std::string("matrices.modelview_matrix"), cameraSpaceMatrix * modelMatrix);
	shader->setProperty(std::string("matrices.normal_matrix"), glm::mat3(cameraSpaceMatrix));
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
ClipmapTerrainSystem::_drawRing(
	ClipmapTerrain* terrain, 
	int i, 
	int numRings, 
	std::shared_ptr<G2::Shader> shader,
	glm::mat4 const& cameraSpaceMatrix,
	glm::vec4 const& lastGridCamPos, 
	glm::vec4 const& camPos
) 
{
	/********************************************
	 * Draw the clipmap ring i - overview	    *
	 ********************************************
	 * Here we want to draw the clipmap ring i. *
	 * Therefore we have to draw all the parts  *
	 * of it. Also we have to track each		*
	 * translation in worldspace to adjust the  *
	 * texture coordinates the same way. We also*
	 * have to sync all variables so the shader *
	 * when drawing the ring.					*
	 *******************************************/
	float w = (float)terrain->mHeightMap->getWidth();
	float h = (float)terrain->mHeightMap->getHeight();
	
	glm::vec2 texCoordOffset(0.f,0.f);
	
	// initially shift the texture coordinate to make the rings come together at the right texture coordinate
	if( i != 1 ) 
	{
		texCoordOffset += terrain->_getJacobsthalNumberTexCoordOffset(i);
	}
	glm::vec2 cachedTexCoordOffset = texCoordOffset;
	float scale = std::pow(2.f, (float)(i-1));

	glm::mat4 modelMatrix = (i%2 == 0 
		? 
		glm::translate(
			-terrain->_getJacobsthalNumber(i)* terrain->mTileSize, 
			0.0f,  
			-terrain->_getJacobsthalNumber(i) * terrain->mTileSize
		) 
		:
		glm::translate(
			terrain->_getJacobsthalNumber(i)* terrain->mTileSize, 
			0.0f,  
			terrain->_getJacobsthalNumber(i) * terrain->mTileSize
		)
	);
	modelMatrix = modelMatrix * glm::scale(scale,1.f,scale);
	modelMatrix = modelMatrix * glm::translate(terrain->mClipmapWidth * terrain->mTileSize, 0.f, terrain->mClipmapWidth * terrain->mTileSize);
	modelMatrix = modelMatrix * glm::translate(-(2.f*terrain->mClipmapWidth) * terrain->mTileSize, 0.f, -(2.f*terrain->mClipmapWidth) * terrain->mTileSize);
	
	texCoordOffset += glm::vec2(((terrain->mClipmapWidth-1.f))/w,((terrain->mClipmapWidth-1.f))/h);
	cachedTexCoordOffset = texCoordOffset;
	glm::mat4 cachedModelMatrix = modelMatrix;

	{
		// set degenerate color
		shader->setProperty("material.ambient", glm::vec4(0.2f,(float)i / (float)numRings,0.1f,1.f));

		// prepare and draw top outer degenerate
		modelMatrix = modelMatrix * glm::translate((3.f*terrain->mClipmapWidth-1.f) * terrain->mTileSize, 0.f, -(terrain->mClipmapWidth-1.f) * terrain->mTileSize);
		_uploadMatrices(shader, cameraSpaceMatrix, modelMatrix);
		texCoordOffset -= glm::vec2(((3.f*terrain->mClipmapWidth-1.f))/w,(-(terrain->mClipmapWidth-1.f))/h);
		shader->setProperty( "_texCoordOffset", texCoordOffset );
		GLDEBUG( glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, terrain->mOuterDegenerateX.id ) );
		GLDEBUG( glDrawElements( GL_TRIANGLES, terrain->mOuterDegenerateX.numIndices, GL_UNSIGNED_INT, NULL) );
		GLDEBUG( glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, NULL) );
		
		// prepare and draw bottom and left outer degenerate
		modelMatrix = modelMatrix * glm::translate(0.f, 0.f, (4.f*(terrain->mClipmapWidth-1.f) + 2.f) * terrain->mTileSize);
		_uploadMatrices(shader, cameraSpaceMatrix, modelMatrix);
		texCoordOffset -= glm::vec2(0.f,(4.f*(terrain->mClipmapWidth-1.f) + 2.f)/h);
		shader->setProperty( "_texCoordOffset", texCoordOffset );
		GLDEBUG( glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, terrain->mOuterDegenerateX.id ) );
		GLDEBUG( glDrawElements( GL_TRIANGLES, terrain->mOuterDegenerateX.numIndices, GL_UNSIGNED_INT, NULL) );
		GLDEBUG( glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, NULL) );
		GLDEBUG( glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, terrain->mOuterDegenerateZ.id ) );
		GLDEBUG( glDrawElements( GL_TRIANGLES, terrain->mOuterDegenerateZ.numIndices, GL_UNSIGNED_INT, NULL) );
		GLDEBUG( glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, NULL) );
		
		// prepare and draw right outer degenerate
		modelMatrix = modelMatrix * glm::translate(-(4.f*(terrain->mClipmapWidth-1.f) + 2.f) * terrain->mTileSize, 0.f, 0.f);
		_uploadMatrices(shader, cameraSpaceMatrix, modelMatrix);
		texCoordOffset -= glm::vec2(-(4.f*(terrain->mClipmapWidth-1.f) + 2.f)/w,0.f);
		shader->setProperty( "_texCoordOffset", texCoordOffset );
		GLDEBUG( glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, terrain->mOuterDegenerateZ.id ) );
		GLDEBUG( glDrawElements( GL_TRIANGLES, terrain->mOuterDegenerateZ.numIndices, GL_UNSIGNED_INT, NULL) );
		GLDEBUG( glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, NULL) );
	}

	// prepare and render mxm blocks
	texCoordOffset = cachedTexCoordOffset;
	modelMatrix = cachedModelMatrix;
	shader->setProperty("material.ambient", glm::vec4((float)i / (float)numRings,0.3f,0.1f,1.f));
	{
		shader->setProperty( "_texCoordOffset", texCoordOffset );
		_uploadMatrices(shader, cameraSpaceMatrix, modelMatrix);
		GLDEBUG( glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, terrain->mMMInstanceBlock.id ) );
		GLDEBUG( glDrawElements( GL_TRIANGLE_STRIP, terrain->mMMInstanceBlock.numIndices, GL_UNSIGNED_INT, NULL) );
		GLDEBUG( glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, NULL) );

		modelMatrix = modelMatrix * glm::translate((terrain->mClipmapWidth-1.f) * terrain->mTileSize, 0.f, 0.f);
		_uploadMatrices(shader, cameraSpaceMatrix, modelMatrix);
		texCoordOffset -= glm::vec2((terrain->mClipmapWidth-1.f)/w,0.f);
		shader->setProperty( "_texCoordOffset", texCoordOffset );
		GLDEBUG( glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, terrain->mMMInstanceBlock.id ) );
		GLDEBUG( glDrawElements( GL_TRIANGLE_STRIP, terrain->mMMInstanceBlock.numIndices, GL_UNSIGNED_INT, NULL) );
		GLDEBUG( glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, NULL) );

		modelMatrix = modelMatrix * glm::translate((terrain->mClipmapWidth-1.f+2.f) * terrain->mTileSize, 0.f, 0.f);
		_uploadMatrices(shader, cameraSpaceMatrix, modelMatrix);
		texCoordOffset -= glm::vec2((terrain->mClipmapWidth-1.f+2.f)/w,0.f);
		shader->setProperty( "_texCoordOffset", texCoordOffset );
		GLDEBUG( glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, terrain->mMMInstanceBlock.id ) );
		GLDEBUG( glDrawElements( GL_TRIANGLE_STRIP, terrain->mMMInstanceBlock.numIndices, GL_UNSIGNED_INT, NULL) );
		GLDEBUG( glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, NULL) );

		modelMatrix = modelMatrix * glm::translate((terrain->mClipmapWidth-1.f) * terrain->mTileSize, 0.f, 0.f);
		_uploadMatrices(shader, cameraSpaceMatrix, modelMatrix);
		texCoordOffset -= glm::vec2((terrain->mClipmapWidth-1.f)/w,0.f);
		shader->setProperty( "_texCoordOffset", texCoordOffset );
		GLDEBUG( glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, terrain->mMMInstanceBlock.id ) );
		GLDEBUG( glDrawElements( GL_TRIANGLE_STRIP, terrain->mMMInstanceBlock.numIndices, GL_UNSIGNED_INT, NULL) );
		GLDEBUG( glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, NULL) );

		modelMatrix = modelMatrix * glm::translate(0.f, 0.f, (terrain->mClipmapWidth-1.f) * terrain->mTileSize);
		_uploadMatrices(shader, cameraSpaceMatrix, modelMatrix);
		texCoordOffset -= glm::vec2(0.f,(terrain->mClipmapWidth-1.f)/h);
		shader->setProperty( "_texCoordOffset", texCoordOffset );
		GLDEBUG( glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, terrain->mMMInstanceBlock.id ) );
		GLDEBUG( glDrawElements( GL_TRIANGLE_STRIP, terrain->mMMInstanceBlock.numIndices, GL_UNSIGNED_INT, NULL) );
		GLDEBUG( glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, NULL) );

		modelMatrix = modelMatrix * glm::translate(0.f, 0.f, (terrain->mClipmapWidth-1.f+2.f) * terrain->mTileSize);
		_uploadMatrices(shader, cameraSpaceMatrix, modelMatrix);
		texCoordOffset -= glm::vec2(0.f,(terrain->mClipmapWidth-1.f+2.f)/h);
		shader->setProperty( "_texCoordOffset", texCoordOffset );
		GLDEBUG( glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, terrain->mMMInstanceBlock.id ) );
		GLDEBUG( glDrawElements( GL_TRIANGLE_STRIP, terrain->mMMInstanceBlock.numIndices, GL_UNSIGNED_INT, NULL) );
		GLDEBUG( glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, NULL) );


		modelMatrix = modelMatrix * glm::translate(0.f, 0.f, (terrain->mClipmapWidth-1.f) * terrain->mTileSize);
		_uploadMatrices(shader, cameraSpaceMatrix, modelMatrix);
		texCoordOffset -= glm::vec2(0.f,(terrain->mClipmapWidth-1.f)/h);
		shader->setProperty( "_texCoordOffset", texCoordOffset );
		GLDEBUG( glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, terrain->mMMInstanceBlock.id ) );
		GLDEBUG( glDrawElements( GL_TRIANGLE_STRIP, terrain->mMMInstanceBlock.numIndices, GL_UNSIGNED_INT, NULL) );
		GLDEBUG( glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, NULL) );
	}
	texCoordOffset = cachedTexCoordOffset;
	modelMatrix = cachedModelMatrix;
	{
		modelMatrix = modelMatrix * glm::translate(0.f, 0.f, (terrain->mClipmapWidth-1.f) * terrain->mTileSize);
		_uploadMatrices(shader, cameraSpaceMatrix, modelMatrix);
		texCoordOffset -= glm::vec2(0.f,(terrain->mClipmapWidth-1.f)/h);
		shader->setProperty( "_texCoordOffset", texCoordOffset );
		GLDEBUG( glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, terrain->mMMInstanceBlock.id ) );
		GLDEBUG( glDrawElements( GL_TRIANGLE_STRIP, terrain->mMMInstanceBlock.numIndices, GL_UNSIGNED_INT, NULL) );
		GLDEBUG( glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, NULL) );

		modelMatrix = modelMatrix * glm::translate(0.f, 0.f, (terrain->mClipmapWidth-1.f+2.f) * terrain->mTileSize);
		_uploadMatrices(shader, cameraSpaceMatrix, modelMatrix);
		texCoordOffset -= glm::vec2(0.f,(terrain->mClipmapWidth-1.f+2.f)/h);
		shader->setProperty( "_texCoordOffset", texCoordOffset );
		GLDEBUG( glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, terrain->mMMInstanceBlock.id ) );
		GLDEBUG( glDrawElements( GL_TRIANGLE_STRIP, terrain->mMMInstanceBlock.numIndices, GL_UNSIGNED_INT, NULL) );
		GLDEBUG( glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, NULL) );

		modelMatrix = modelMatrix * glm::translate(0.f, 0.f, (terrain->mClipmapWidth-1.f) * terrain->mTileSize);
		_uploadMatrices(shader, cameraSpaceMatrix, modelMatrix);
		texCoordOffset -= glm::vec2(0.f,(terrain->mClipmapWidth-1.f)/h);
		shader->setProperty( "_texCoordOffset", texCoordOffset );
		GLDEBUG( glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, terrain->mMMInstanceBlock.id ) );
		GLDEBUG( glDrawElements( GL_TRIANGLE_STRIP, terrain->mMMInstanceBlock.numIndices, GL_UNSIGNED_INT, NULL) );
		GLDEBUG( glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, NULL) );

		modelMatrix = modelMatrix * glm::translate((terrain->mClipmapWidth-1.f) * terrain->mTileSize, 0.f, 0.f);
		_uploadMatrices(shader, cameraSpaceMatrix, modelMatrix);
		texCoordOffset -= glm::vec2((terrain->mClipmapWidth-1.f)/w,0.f);
		shader->setProperty( "_texCoordOffset", texCoordOffset );
		GLDEBUG( glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, terrain->mMMInstanceBlock.id ) );
		GLDEBUG( glDrawElements( GL_TRIANGLE_STRIP, terrain->mMMInstanceBlock.numIndices, GL_UNSIGNED_INT, NULL) );
		GLDEBUG( glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, NULL) );

		modelMatrix = modelMatrix * glm::translate((terrain->mClipmapWidth-1.f+2.f) * terrain->mTileSize, 0.f, 0.f);
		_uploadMatrices(shader, cameraSpaceMatrix, modelMatrix);
		texCoordOffset -= glm::vec2((terrain->mClipmapWidth-1.f+2.f)/w,0.f);
		shader->setProperty( "_texCoordOffset", texCoordOffset );
		GLDEBUG( glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, terrain->mMMInstanceBlock.id ) );
		GLDEBUG( glDrawElements( GL_TRIANGLE_STRIP, terrain->mMMInstanceBlock.numIndices, GL_UNSIGNED_INT, NULL) );
		GLDEBUG( glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, NULL) );
	}
	
	// prepare and render ring fixups on X-Axis
	texCoordOffset = cachedTexCoordOffset;
	modelMatrix = cachedModelMatrix;
	shader->setProperty("material.ambient", glm::vec4(0.3f,0.1f,(float)i / (float)numRings,1.f));
	{
		modelMatrix = modelMatrix * glm::translate(0.f, 0.f, (terrain->mClipmapWidth+1.f) * terrain->mTileSize);
		_uploadMatrices(shader, cameraSpaceMatrix, modelMatrix);
		texCoordOffset -= glm::vec2(0.f,(terrain->mClipmapWidth+1.f)/h);
		shader->setProperty( "_texCoordOffset", texCoordOffset );
		GLDEBUG( glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, terrain->mRingFixupX.id ) );
		GLDEBUG( glDrawElements( GL_TRIANGLE_STRIP, terrain->mRingFixupX.numIndices, GL_UNSIGNED_INT, NULL) );
		GLDEBUG( glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, NULL) );

		modelMatrix = modelMatrix * glm::translate((3.f * terrain->mClipmapWidth - 1.f) * terrain->mTileSize, 0.f, 0.f);
		_uploadMatrices(shader, cameraSpaceMatrix, modelMatrix);
		texCoordOffset -= glm::vec2((3.f*terrain->mClipmapWidth-1.f)/w,0.f);
		shader->setProperty( "_texCoordOffset", texCoordOffset );
		GLDEBUG( glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, terrain->mRingFixupX.id ) );
		GLDEBUG( glDrawElements( GL_TRIANGLE_STRIP, terrain->mRingFixupX.numIndices, GL_UNSIGNED_INT, NULL) );
		GLDEBUG( glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, NULL) );
	}
	
	// prepare and render ring fixups on Z-Axis
	texCoordOffset = cachedTexCoordOffset;
	modelMatrix = cachedModelMatrix;
	{
		modelMatrix = modelMatrix * glm::translate((terrain->mClipmapWidth+1.f) * terrain->mTileSize, 0.f, 0.f);
		_uploadMatrices(shader, cameraSpaceMatrix, modelMatrix);
		texCoordOffset -= glm::vec2((terrain->mClipmapWidth+1.f)/w,0.f);
		shader->setProperty( "_texCoordOffset", texCoordOffset );
		GLDEBUG( glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, terrain->mRingFixupZ.id ) );
		GLDEBUG( glDrawElements( GL_TRIANGLE_STRIP, terrain->mRingFixupZ.numIndices, GL_UNSIGNED_INT, NULL) );
		GLDEBUG( glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, NULL) );

		modelMatrix = modelMatrix * glm::translate(0.f, 0.f, (3.f * terrain->mClipmapWidth - 1.f) * terrain->mTileSize);
		_uploadMatrices(shader, cameraSpaceMatrix, modelMatrix);
		texCoordOffset -= glm::vec2(0.f,(3.f*terrain->mClipmapWidth-1.f)/h);
		shader->setProperty( "_texCoordOffset", texCoordOffset );
		GLDEBUG( glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, terrain->mRingFixupZ.id ) );
		GLDEBUG( glDrawElements( GL_TRIANGLE_STRIP, terrain->mRingFixupZ.numIndices, GL_UNSIGNED_INT, NULL) );
		GLDEBUG( glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, NULL) );
	}
	
	// prepare and render ring fixups on Z-Axis
	texCoordOffset = cachedTexCoordOffset;
	modelMatrix = cachedModelMatrix;
	shader->setProperty("material.ambient", glm::vec4(0.3f,(float)i / (float)numRings,(float)i / (float)numRings,1.f));
	if(i > 1)
	{
		float add = 0.0f; // we add 1.0 in case that the left interior trim is active to adjust the position of the top or bottom interior trim
		texCoordOffset = cachedTexCoordOffset;
		modelMatrix = cachedModelMatrix;
		if(i%2 == 0)
		{
			if(lastGridCamPos.x == camPos.x)
			{
				// left interior trim is active
				modelMatrix = modelMatrix * glm::translate(terrain->mTileSize, 0.f, 2.f * terrain->mClipmapWidth * terrain->mTileSize);
				_uploadMatrices(shader, cameraSpaceMatrix, modelMatrix);
				texCoordOffset -= glm::vec2(1.f/w,(2.f*terrain->mClipmapWidth)/h);
				shader->setProperty( "_texCoordOffset", texCoordOffset );
				GLDEBUG( glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, terrain->mInteriorTrimZ.id ) );
				GLDEBUG( glDrawElements( GL_TRIANGLE_STRIP, terrain->mInteriorTrimZ.numIndices, GL_UNSIGNED_INT, NULL) );
				GLDEBUG( glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, NULL) );
				add = 1.0f; // top or bottom interior trim have to move to the right
			}
			else
			{
				// right interior trim is active
				modelMatrix = modelMatrix * glm::translate(2.f * terrain->mClipmapWidth * terrain->mTileSize, 0.f, 2.f * terrain->mClipmapWidth * terrain->mTileSize);
				_uploadMatrices(shader, cameraSpaceMatrix, modelMatrix);
				texCoordOffset -= glm::vec2((2.f * terrain->mClipmapWidth)/w,(2.f * terrain->mClipmapWidth)/h);
				shader->setProperty( "_texCoordOffset", texCoordOffset );
				GLDEBUG( glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, terrain->mInteriorTrimZ.id ) );
				GLDEBUG( glDrawElements( GL_TRIANGLE_STRIP, terrain->mInteriorTrimZ.numIndices, GL_UNSIGNED_INT, NULL) );
				GLDEBUG( glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, NULL) );
			}
			texCoordOffset = cachedTexCoordOffset;
			modelMatrix = cachedModelMatrix;
			if(lastGridCamPos.z == camPos.z)
			{
				// top interior trim is active
				modelMatrix = modelMatrix * glm::translate((2.f * terrain->mClipmapWidth - 1.f + add) * terrain->mTileSize, 0.f, terrain->mTileSize);
				_uploadMatrices(shader, cameraSpaceMatrix, modelMatrix);
				texCoordOffset -= glm::vec2((2.f * terrain->mClipmapWidth - 1.f + add)/w,1.f/h);
				shader->setProperty( "_texCoordOffset", texCoordOffset );
				GLDEBUG( glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, terrain->mInteriorTrimX.id ) );
				GLDEBUG( glDrawElements( GL_TRIANGLE_STRIP, terrain->mInteriorTrimX.numIndices, GL_UNSIGNED_INT, NULL) );
				GLDEBUG( glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, NULL) );
			}
			else
			{
				// bottom interior trim is active
				modelMatrix = modelMatrix * glm::translate((2.f * terrain->mClipmapWidth - 1.f + add) * terrain->mTileSize, 0.f, 2.f * terrain->mClipmapWidth * terrain->mTileSize);
				_uploadMatrices(shader, cameraSpaceMatrix, modelMatrix);
				texCoordOffset -= glm::vec2((2.f * terrain->mClipmapWidth - 1.f + add)/w,(2.f * terrain->mClipmapWidth)/h);
				shader->setProperty( "_texCoordOffset", texCoordOffset );
				GLDEBUG( glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, terrain->mInteriorTrimX.id ) );
				GLDEBUG( glDrawElements( GL_TRIANGLE_STRIP, terrain->mInteriorTrimX.numIndices, GL_UNSIGNED_INT, NULL) );
				GLDEBUG( glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, NULL) );
			}
		}
		else
		{
			if(lastGridCamPos.x != camPos.x) // ONLY DIFFERENCE HERE!
			{
				// left interior trim is active
				modelMatrix = modelMatrix * glm::translate(terrain->mTileSize, 0.f, 2.f * terrain->mClipmapWidth * terrain->mTileSize);
				_uploadMatrices(shader, cameraSpaceMatrix, modelMatrix);
				texCoordOffset -= glm::vec2(1.f/w,(2.f*terrain->mClipmapWidth)/h);
				shader->setProperty( "_texCoordOffset", texCoordOffset );
				GLDEBUG( glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, terrain->mInteriorTrimZ.id ) );
				GLDEBUG( glDrawElements( GL_TRIANGLE_STRIP, terrain->mInteriorTrimZ.numIndices, GL_UNSIGNED_INT, NULL) );
				GLDEBUG( glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, NULL) );
				add = 1.0f; // top or bottom interior trim have to move to the right
			}
			else
			{
				// right interior trim is active
				modelMatrix = modelMatrix * glm::translate(2.f * terrain->mClipmapWidth * terrain->mTileSize, 0.f, 2.f * terrain->mClipmapWidth * terrain->mTileSize);
				_uploadMatrices(shader, cameraSpaceMatrix, modelMatrix);
				texCoordOffset -= glm::vec2((2.f * terrain->mClipmapWidth)/w,(2.f * terrain->mClipmapWidth)/h);
				shader->setProperty( "_texCoordOffset", texCoordOffset );
				GLDEBUG( glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, terrain->mInteriorTrimZ.id ) );
				GLDEBUG( glDrawElements( GL_TRIANGLE_STRIP, terrain->mInteriorTrimZ.numIndices, GL_UNSIGNED_INT, NULL) );
				GLDEBUG( glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, NULL) );
			}
			texCoordOffset = cachedTexCoordOffset;
			modelMatrix = cachedModelMatrix;
			if(lastGridCamPos.z != camPos.z)
			{
				// top interior trim is active
				modelMatrix = modelMatrix * glm::translate((2.f * terrain->mClipmapWidth - 1.f + add) * terrain->mTileSize, 0.f, terrain->mTileSize);
				_uploadMatrices(shader, cameraSpaceMatrix, modelMatrix);
				texCoordOffset -= glm::vec2((2.f * terrain->mClipmapWidth - 1.f + add)/w,1.f/h);
				shader->setProperty( "_texCoordOffset", texCoordOffset );
				GLDEBUG( glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, terrain->mInteriorTrimX.id ) );
				GLDEBUG( glDrawElements( GL_TRIANGLE_STRIP, terrain->mInteriorTrimX.numIndices, GL_UNSIGNED_INT, NULL) );
				GLDEBUG( glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, NULL) );
			}
			else
			{
				// bottom interior trim is active
				modelMatrix = modelMatrix * glm::translate((2.f * terrain->mClipmapWidth - 1.f + add) * terrain->mTileSize, 0.f, 2.f * terrain->mClipmapWidth * terrain->mTileSize);
				_uploadMatrices(shader, cameraSpaceMatrix, modelMatrix);
				texCoordOffset -= glm::vec2((2.f * terrain->mClipmapWidth - 1.f + add)/w,(2.f * terrain->mClipmapWidth)/h);
				shader->setProperty( "_texCoordOffset", texCoordOffset );
				GLDEBUG( glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, terrain->mInteriorTrimX.id ) );
				GLDEBUG( glDrawElements( GL_TRIANGLE_STRIP, terrain->mInteriorTrimX.numIndices, GL_UNSIGNED_INT, NULL) );
				GLDEBUG( glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, NULL) );
			}
		}
	}
}