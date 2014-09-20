// GEAR 2.5 - Game Engine Andy Reimann - Author: Andy Reimann <andy@moorlands-grove.de>
// (c) 2014 GEAR 2.5
#pragma once
#include "Defines.h"
#include "RefCounter.h"
#include "VersionTracker.h"

#include <G2Core/Defines.h>

#include <glm/glm.hpp>
#include <memory>
#include <array>
#include <vector>

namespace G2
{
	class Shader;
	/** This class defines a VertexArrayObject, combining buffers for 
	 * vertices, normals and user defined data.
	 * @created:	2014/01/22
	 * @author Andy Reimann <a.reimann@moorlands-grove.de>
	 */
	class VertexArrayObject : public VersionTracker
	{
			friend class RenderSystem;
			friend class RenderComponent;
		public:
			
			/** This constructs a new VertexArrayObject.
			 * By default no space is allocated and reserved. Make sure to call resizeElementCount.
			 */
			VertexArrayObject();
			/** Copy ctor.
			 * @warning Copying a VertexArrayObject will share the OpenGL-Buffer.
			 * Therefore both VertexArrayObjects will write into the same OpenGL resource!
			 */
			VertexArrayObject(VertexArrayObject const& rhs);
			/** Copy assignment operator.
			 * @warning Copying a VertexArrayObject will share the OpenGL-Buffer.
			 * Therefore both VertexArrayObjects will write into the same OpenGL resource!
			 */
			VertexArrayObject& operator=(VertexArrayObject const& rhs);
			/** Move ctor.
			 */
			VertexArrayObject(VertexArrayObject && rhs);
			/** Move assignment operator.
			 */
			VertexArrayObject& operator=(VertexArrayObject && rhs);
			/** This constructs a new VertexArrayObject.
			 * @param numElements The number of elements to create the VertexArrayObject for.
			 * @note When creating a VertexArrayObject, no space for data will be allocated.
			 * @see allocateVertexData and allocateNormalData
			 */
			VertexArrayObject(unsigned int numElements);
			/** Resizes the VertexArrayObject and all of its allocated spaces.
			 * @param numElements The new amount of space to allocate.
			 * @note The VertexArrayObject will try to not reallocate the space if possible.
			 * Therefore it will hold the biggest requested space as long as possible even
			 * if you later request a resize to a smaller size.
			 * @warning Previously written data may be lost!
			 * @return The calling VertexArrayObject instance.
			 */
			VertexArrayObject& resizeElementCount(unsigned int numElements);
			
			VertexArrayObject& writeData(G2Core::Semantics::Name semantic, glm::vec2 const* data, int numElements = -1);
			VertexArrayObject& writeData(G2Core::Semantics::Name semantic, glm::vec3 const* data, int numElements = -1);
			VertexArrayObject& writeData(G2Core::Semantics::Name semantic, glm::vec4 const* data, int numElements = -1);

			
			float* getDataPointer(G2Core::Semantics::Name semantic, G2Core::BufferAccessMode::Name mode = G2Core::BufferAccessMode::WRITE_ONLY);
			void returnDataPointer(G2Core::Semantics::Name semantic);

			/** This function will return the NumElements. 
			* @return The current NumElements.
			*/
			unsigned int getNumElements() const { return mNumElements; }
			/** Returns the number of bytes used by one value in the given semantic.
			 * @note If the semantic is not used in the VertexArrayObject, 0 is returned.
			 * @return The number of bytes used by one value in the given semantic.
			 */
			unsigned int getNumBytesBySemantic(G2Core::Semantics::Name semantic) const;

			void bind();
			/** Draws the VertexArrayObject with the given OpenGL draw mode.
			 * @param drawMode The draw mode to use.
			 */
			void draw(std::shared_ptr<G2::Shader> boundShader, G2Core::DrawMode::Name drawMode, int numVertices = -1);
			void unbind();

			~VertexArrayObject();
		private:

			void _initVAOBuffer();
			void _deleteBuffers();
			
			unsigned int	mNumElements;			// The number of elements the VertexArrayObject manages
			unsigned int	mMaxNumElements;		// The maximum number of elements the VertexArrayObject can manage

			G2Core::GfxResource* mVertexArrayResource; // The resource handle in the GfxContext
			std::array<unsigned int,G2Core::Semantics::NUM_SEMANTICS> mBytesPerSemantic; // The number of bytes, the semantic has allocated
			std::shared_ptr<RefCounter<int>>		mReferenceCounter; // A reference counter if this instance is shared by copies
	};
};
