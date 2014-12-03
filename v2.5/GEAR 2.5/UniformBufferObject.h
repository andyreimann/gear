#pragma once

#include <G2Core/GfxDevice.h>
#include <G2Core/NonCopyable.h>

namespace G2 
{
	/** This class defines a Uniform Buffer Object.
	 * @created:	2014/12/03
	 * @author Andy Reimann <a.reimann@moorlands-grove.de>
	 */
	class UniformBufferObject : public G2::NonCopyable
	{
		friend class Shader;
		public:
			/** This constructs a new UniformBufferObject.
			 */
			UniformBufferObject(std::string const& shadingLanguage, unsigned int byteSize, void* data, G2Core::BufferUsage::Name bufferUsage, G2Core::UniformBufferBindingPoint::Name uniformBufferBindingPoint);

			/** Move ctor.
			 */
			UniformBufferObject(UniformBufferObject && rhs);
			/** Move assignment operator.
			 */
			UniformBufferObject& operator=(UniformBufferObject && rhs);
			/// normal destructor
			~UniformBufferObject();

			void bind();
			/** Updates the data inside the UniformBufferObject.
			 * @param byteOffset The offset in bytes were you want to start writing the new data.
			 * @param data The number of indices to draw or -1 = default if you just want to draw all indices.
			 * @note Make sure to bind a UniformBufferObject before calling the updateSubData function.
			 */
			void updateSubData(unsigned int byteOffset, unsigned int byteSize, void* data);
			void unbind();
		private:
			/** Copy ctor.
			* @warning Copying a UniformBufferObject will share the OpenGL-Buffer.
			* Therefore both UniformBufferObject will write into the same OpenGL resource!
			*/
			UniformBufferObject(UniformBufferObject const& rhs) {}
			UniformBufferObject& operator=(UniformBufferObject const& rhs) {}

			void _deleteBuffer();
			
			G2Core::GfxResource*				mUniformBufferResource;	// The resource handle in the GfxContext
			unsigned int						mNumBytes;				// The number of bytes the UniformBufferObject manages
			G2Core::BufferUsage::Name			mBufferUsage;			// The buffer usage flag
	};
};