#pragma once
#include "IndexArrayObject.h"

namespace G2 
{
	typedef void(*StreamTriangle)(unsigned int* indices, float* vertexStream, unsigned int triangle, glm::vec3** p1, glm::vec3** p2, glm::vec3** p3);
	/** A TriangleStream provides an easy way of retrieving the vertices of a VertexBuffer 
	 * as Triangles one after each other.
	 * The triangles are therefore extracted depending on the draw mode, buffer type and data type used.
	 * Considering all the different possible combinations, it's a quite difficult problem to solve
	 * resulting in a lot of duplicate code. The TriangleStream class solves this problem by providing a uniform
	 * way of fetching the data.
	 * @created:	2014/11/27
	 * @author Andy Reimann <a.reimann@moorlands-grove.de>
	 */
	class TriangleStream 
	{
		public:
			/** Constructs and initializes a new TriangleStream object.
			 * @param semantic The semantic to stream the triangle data from.
			 * @param iao An optional existing IndexArrayObject to use or a nullptr otherwise.
			 * @param vao The VertexArrayObject to use for the TriangleStream.
			 * @param drawMode The draw mode to use for streaming the triangle data.
			 * @note The constructor acquires all resource pointers to the data and the destructor releases them. 
			 * Therefore make sure to delete a TriangleStream object as early as possible.
			 * @code
			 // it's a good idea to put a TriangleStream into a defined scope to ensure a controlled release of data
			 {
				TriangleStream stream(G2Core::Semantics::POSITION, iao, vao, G2Core::DrawMode::TRIANGLES);

				// ... do work

				// at end of scope the data is released
			 }
			 * @endcode
			 */
			TriangleStream(
				G2Core::Semantics::Name semantic,
				G2::IndexArrayObject* iao, 
				G2::VertexArrayObject* vao,
				G2Core::DrawMode::Name drawMode);
			/** Checks whether a next triangle can be streamed.
			 * @return True if a next triangle can be streamed, false otherwise.
			 */
			bool hasNext();
			/** Streams the next triangle.
			 * @param p1 A pointer which will point to the first point of the triangle afterwards.
			 * @param p2 A pointer which will point to the first point of the triangle afterwards.
			 * @param p3 A pointer which will point to the first point of the triangle afterwards.
			 * @note This function does not check whether a next triangle can actually be pulled. See hasNext for more information.
			 */
			void next(glm::vec3** p1, glm::vec3** p2, glm::vec3** p3);
			/** Deletes the TriangleStream and releases any connection to the stream data.
			 * 
			 */
			~TriangleStream();

		protected:
		private:

			unsigned int			mNumTriangles;		// Total number of triangles in the stream
			unsigned int			mStreamedTriangles; // Number of already streamed triangles

			unsigned int*			mIndexStream;		// The optional existing index stream
			float*					mVertexStream;			// The vertex data stream

			G2::IndexArrayObject*	mIndexArrayObject;	// An optional IndexArrayObject
			G2::VertexArrayObject*	mVertexArrayObject;	// The VertexArrayObject of the TriangleStream.
			G2Core::DrawMode::Name	mDrawMode;			// The draw mode which is configured
			G2Core::Semantics::Name mSemantic;			// The semantic to fetch the data from

			StreamTriangle			_streamTriangle;	// The pointer to the function used for fetching the triangles
	};
};