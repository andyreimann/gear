// GEAR 2.5 - Game Engine Andy Reimann - Author: Andy Reimann <andy@moorlands-grove.de>
// (c) 2014 GEAR 2.5
#pragma once
#include "RefCounter.h"
#include "VersionTracker.h"

#include <glm/glm.hpp>
#include <memory>
#include <array>

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
			TEXCOORD_6,
			TEXCOORD_7,
			TANGENT,
			BINORMAL,
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
	/** This class defines a VertexArrayObject, combining buffers for 
	 * vertices, normals and user defined data.
	 * @created:	2014/01/22
	 * @author Andy Reimann <a.reimann@moorlands-grove.de>
	 */
	class VertexArrayObject : public VersionTracker
	{
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
			 */
			VertexArrayObject& resize(unsigned int numElements);
			
			VertexArrayObject& writeData(Semantics::Name semantic, glm::vec2 const* data);
			VertexArrayObject& writeData(Semantics::Name semantic, glm::vec3 const* data);
			VertexArrayObject& writeData(Semantics::Name semantic, glm::vec4 const* data);

			
			float* getDataPointer(Semantics::Name semantic);
			void returnDataPointer(Semantics::Name semantic);
			
			VertexArrayObject& writeIndices(unsigned int const* data, unsigned int numIndices);

			/** This function will return the NumElements. 
			* @return The current NumElements.
			*/
			unsigned int getNumElements() const { return mNumElements; }
			/** Draws the VertexArrayObject with the given OpenGL draw mode.
			 * @param glDrawMode The OpenGL draw mode to use.
			 */
			void draw(int glDrawMode);

			~VertexArrayObject();
		private:

			void bind();
			void initVAOBuffer();
			void deleteBuffers();
			void unbind();
			
			bool			mBound;					// Flag if the VertexArrayObject is bound or not.
			unsigned int	mNumElements;			// The number of elements the VertexArrayObject manages
			unsigned int	mMaxNumElements;		// The maximum number of elements the VertexArrayObject can manage

			unsigned int	mVertexArrayId;			// The ID of the OpenGL VertexArrayObject

			unsigned int	mIndexBufferId;			// The ID of the OpenGL IndexBufferObject
			unsigned int	mNumIndices;			// The amount of indices the IndexBufferObject holds

			std::array<unsigned int,Semantics::NUM_SEMANTICS> mBufferIds; // The IDs for the available semantics
			std::array<unsigned int,Semantics::NUM_SEMANTICS> mBytesPerSemantic; // The number of bytes, the semantic has allocated
			std::shared_ptr<RefCounter<int>>		mReferenceCounter;
	};
};
