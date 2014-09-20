#pragma once
#include "VertexArrayObject.h"

#include <G2Core/NonCopyable.h>
#include <vector>

namespace G2 
{
	/// This class defines...
	/// @created:	2014/05/23
	/// @author Andy Reimann <a.reimann@moorlands-grove.de>
	class VertexStream 
	{
		public:
			/// This constructs a new VertexStream.
			/// The vertices are fetched from the given vao in the given Semantics.
			VertexStream(int num, int componentsPerVertex);
			/// Move constructor.
			VertexStream(VertexStream && rhs);
			/// Basic move operator.
			VertexStream& operator=(VertexStream && rhs);
			/// This will write all available vertices from the given vao in the given
			/// semantic to the VertexStream.
			/// If the VertexSteam is full, it will silently stop writing data.
			void write(VertexArrayObject& vao, G2Core::Semantics::Name semantic);
			/// This will write all available triangles from the given vao.
			/// If the VertexSteam is full, it will silently stop writing data.
			void writeTriangles(VertexArrayObject& vao);
			/// This will write all available triangles from the given vao.
			/// If the VertexSteam is full, it will silently stop writing data.
			/// Each vertex will be multiplied with the given transformation matrix before it is written.
			void writeTriangles(VertexArrayObject& vao, glm::mat4 const& transformation);

			float const* get() const { return &mVertexStream[0]; };

			unsigned int getNumWrittenVertices() const { return mWriteIndex * sizeof(float) / mBytesPerVertex; }
		private:
			unsigned int		mBytesPerVertex;
			std::vector<float>	mVertexStream; // holds only vec3!
			unsigned int		mWriteIndex;
	};
};