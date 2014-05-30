//// GEAR 2.5 - Game Engine Andy Reimann - Author: Andy Reimann <andy@moorlands-grove.de>
//// (c) 2014 GEAR 2.5
//#pragma once
//#include <glm/glm.hpp>
//#include "Defines.h"
//
//namespace G2
//{
//	class VertexBuffer
//	{
//		public:
//			VertexBuffer(int numVertices, glm::vec4* vertices, glm::vec4* colors);
//			VertexBuffer(int numVertices, glm::vec4* vertices, glm::vec4* colors, glm::vec3* normals);
//
//
//			~VertexBuffer(void);
//		
//			void bind();
//		
//			void draw(int glDrawMode);
//
//			void unbind();
//
//			void lock();
//			/** This function will return the number of vertices this VertexBuffer holds.
//			* @return The number of vertices this VertexBuffer holds.
//			*/
//			unsigned const& getNumVertices() const { return mNumVertices; }
//
//			float* getDataPointer();
//
//			void unlock();
//
//		private:
//
//			unsigned mNumVertices;		//!< the number of vertices, the buffer contains
//			
//			unsigned mNumFloats;		//!< the number of floats of the buffer including all the data of the additional buffers data.
//			unsigned mVao;
//			unsigned mVbo;	// vertices
//			unsigned mCbo;	// colors
//			unsigned mNbo;	// normals
//			float*	 mBufferData;
//	};
//};
