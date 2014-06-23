// GEAR 2.5 - Game Engine Andy Reimann - Author: Andy Reimann <andy@moorlands-grove.de>
// (c) 2014 GEAR 2.5
#pragma once
#include "Defines.h"
#include "RefCounter.h"
#include "VersionTracker.h"

#include <glm/glm.hpp>
#include <memory>
#include <array>
#include <vector>

namespace G2
{
	namespace Semantics {
		enum Name {
			POSITION = 0,
			BLENDWEIGHT,
			NORMAL,
			COLOR_0,
			COLOR_1,
			FOGCOORD,
			PSIZE,
			BLENDINDICES,
			TEXCOORD_0,
			TEXCOORD_1,
			TEXCOORD_2,
			TEXCOORD_3,
			TEXCOORD_4,
			TEXCOORD_5,
			BINORMAL,
			TANGENT,
			NUM_SEMANTICS,
			SEMANTIC_INVALID
		};
		/** This function will parse the given string to the appropriate
		 * Semantic enum value.
		 * @param name The name to parse.
		 * @return The parsed Semantic enum value.
		 */
		Semantics::Name getSemantic(std::string const& name);
	}
	namespace BufferAccessMode {
		enum Name {
			READ_ONLY = GL_READ_ONLY,
			WRITE_ONLY = GL_WRITE_ONLY,
			READ_WRITE = GL_READ_WRITE,
		};
	}
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
			 * By default no space is allocated and reserved. Make sure to call resize followed
			 * by at least allocateVertexData!
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
			
			VertexArrayObject& writeData(Semantics::Name semantic, glm::vec2 const* data, int numElements = -1);
			VertexArrayObject& writeData(Semantics::Name semantic, glm::vec3 const* data, int numElements = -1);
			VertexArrayObject& writeData(Semantics::Name semantic, glm::vec4 const* data, int numElements = -1);

			
			float* getDataPointer(Semantics::Name semantic, G2::BufferAccessMode::Name mode = BufferAccessMode::WRITE_ONLY);
			void returnDataPointer(Semantics::Name semantic);
			unsigned int* getIndexPointer(unsigned int indexBuffer, G2::BufferAccessMode::Name mode = BufferAccessMode::WRITE_ONLY);
			void returnIndexPointer(unsigned int indexBuffer);
			/** This function will write the given indices into the given index buffer of the VertexArrayObject.
			 * @param indexBuffer The index of the index buffer to write the indices to.
			 * @param data The array containing the indices.
			 * @param numIndices the number of given indices contained in data.
			 * @return The calling VertexArrayObject instance.
			 */
			VertexArrayObject& writeIndices(unsigned int indexBuffer, unsigned int const* data, unsigned int numIndices);
			
			/** This function will return the NumElements. 
			* @return The current NumElements.
			*/
			unsigned int getNumElements() const { return mNumElements; }
			/** This function will return the number of indices. 
			* @return The number of indices.
			*/
			unsigned int getNumIndexBuffers() const { return (unsigned int)mIndexBuffer.size(); };
			/** This function will return the number of indices. 
			* @return The number of indices.
			*/
			unsigned int getNumIndices(unsigned int indexBuffer) const;
			/** This function will return the number of draw calls, the VertexArrayObject will invoke
			 * when the draw() function is called.
			 * @return The  number of draw calls, the VertexArrayObject will invoke
			 * when the draw() function is called.
			 */
			unsigned int getNumDrawCalls() const;
			/** Returns the number of bytes used by one value in the given semantic.
			 * @note If the semantic is not used in the VertexArrayObject, 0 is returned.
			 * @return The number of bytes used by one value in the given semantic.
			 */
			unsigned int getNumBytesBySemantic(Semantics::Name semantic) const;

			bool hasIndexBuffers() const { return mIndexBuffer.size() > 0; }

			void bind();
			/** Draws the VertexArrayObject with the given OpenGL draw mode.
			 * @param glDrawMode The OpenGL draw mode to use.
			 */
			void draw(int glDrawMode, unsigned int drawCall, int numVertices = -1);
			void unbind();

			~VertexArrayObject();
		private:
			/** Resizes the number of index buffers associated with the VertexArrayObject.
			 * By default a VertexArrayObject does not use an index buffer.
			 * If you plan to use one or more index buffers for a VertexArrayObject, you can
			 * allocate them by a call to this function before accessing them.
			 * Using more than one index buffer for an VAO will result in the same amount of draw calls during rendering.
			 * @param numIndexBuffer The new amount of index buffers to allocate.
			 * @note The VertexArrayObject will try to not reallocate the space if possible.
			 * Therefore it will hold the biggest requested space as long as possible even
			 * if you later request a resize to a smaller size.
			 * @warning Previously written data may be lost!
			 * @return The calling VertexArrayObject instance.
			 */
			VertexArrayObject& _resizeIndexBufferCount(unsigned int numIndexBuffer);

			struct IndexBuffer
			{
				IndexBuffer()
					: indexBufferId(GL_INVALID_VALUE),
					numIndices(0) {}
				~IndexBuffer();
				unsigned int	indexBufferId;		// The ID of the OpenGL IndexBufferObject
				unsigned int	numIndices;			// The amount of indices the IndexBufferObject holds
			};

			void _initVAOBuffer();
			void _deleteBuffers();
			
			bool			mBound;					// Flag if the VertexArrayObject is bound or not.
			unsigned int	mNumElements;			// The number of elements the VertexArrayObject manages
			unsigned int	mMaxNumElements;		// The maximum number of elements the VertexArrayObject can manage

			unsigned int	mVertexArrayId;			// The ID of the OpenGL VertexArrayObject

			std::vector<IndexBuffer> mIndexBuffer;	// The index buffer objects associated with this VertexArrayObject

			std::array<unsigned int,Semantics::NUM_SEMANTICS> mBufferIds; // The IDs for the available semantics
			std::array<unsigned int,Semantics::NUM_SEMANTICS> mBytesPerSemantic; // The number of bytes, the semantic has allocated
			std::shared_ptr<RefCounter<int>>		mReferenceCounter;
	};
};
