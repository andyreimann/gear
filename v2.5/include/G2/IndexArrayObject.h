#pragma once
#include "VertexArrayObject.h"

namespace G2 
{
	/** This class defines an array of indices used for an OpenGL draw call.
	 * @created:	2014/06/27
	 * @author Andy Reimann <a.reimann@moorlands-grove.de>
	 */
	class IndexArrayObject : public VersionTracker
	{
		public:
			/** This constructs a new IndexArrayObject.
			 * By default no space is allocated and reserved. Make sure to call resizeElementCount followed
			 * by at least one call to writeIndices!
			 */
			IndexArrayObject();
			/** This constructs a new IndexArrayObject with the given space already allocated and reserved. 
			 */
			IndexArrayObject(unsigned int numElements);
			/** Copy ctor.
			 * @warning Copying a IndexArrayObject will share the OpenGL-Buffer.
			 * Therefore both IndexArrayObject will write into the same OpenGL resource!
			 */
			IndexArrayObject(IndexArrayObject const& rhs);
			/** Move ctor.
			 */
			IndexArrayObject(IndexArrayObject && rhs);
			/** Copy assignment operator.
			 * @warning Copying a IndexArrayObject will share the OpenGL-Buffer.
			 * Therefore both IndexArrayObject will write into the same OpenGL resource!
			 */
			IndexArrayObject& operator=(IndexArrayObject const& rhs);
			/** Move assignment operator.
			 */
			IndexArrayObject& operator=(IndexArrayObject && rhs);
			/// normal destructor
			~IndexArrayObject();
			/** Resizes the IndexArrayObject and all of its allocated spaces.
			 * @param numElements The new amount of space to allocate.
			 * @note The IndexArrayObject will try to not reallocate the space if possible.
			 * Therefore it will hold the biggest requested space as long as possible even
			 * if you later request a resize to a smaller size.
			 * @warning Previously written data may be lost!
			 * @return The calling IndexArrayObject instance.
			 */
			IndexArrayObject& resizeElementCount(unsigned int numElements);

			unsigned int* getIndexPointer(G2::BufferAccessMode::Name mode = BufferAccessMode::WRITE_ONLY);
			void returnIndexPointer();
			

			/** This function will write the given indices into the given index buffer of the IndexArrayObject.
			 * @param data The array containing the indices.
			 * @param numIndices the number of given indices contained in data.
			 * @return The calling IndexArrayObject instance.
			 */
			IndexArrayObject& writeIndices(unsigned int const* data, unsigned int numIndices);
			/** This function will return the number of indices. 
			 * @return The number of indices.
			 */
			unsigned int getNumElements() const { return mNumElements; }

			void bind();
			/** Draws the IndexArrayObject with the given OpenGL draw mode.
			 * @param glDrawMode The OpenGL draw mode to use.
			 * @param numIndices The number of indices to draw or -1 = default if you just want to draw all indices.
			 * @note Make sure to bind a VertexArrayObject as well as the IndexBufferObject before calling the draw function.
			 */
			void draw(int glDrawMode, int numIndices = -1);
			void unbind();
		private:
			void _initIAOBuffer();
			void _deleteBuffer();
			
			bool								mBound;				// Flag if the IndexArrayObject is bound or not.
			unsigned int						mNumElements;		// The number of elements the IndexArrayObject manages
			unsigned int						mMaxNumElements;	// The maximum number of elements the IndexArrayObject can manage

			unsigned int						mIndexArrayId;		// The ID of the OpenGL IndexArrayObject
			std::shared_ptr<RefCounter<int>>	mReferenceCounter;  // A reference counter if this instance is shared by copies 
	};
};