#pragma once

#include <G2Core/GfxDevice.h>
#include <G2Core/NonCopyable.h>

namespace G2 
{
	/** This class defines a Uniform Buffer Object holding some user defined data.
	 * UniformBufferObjects can be used to provide data to a whole bunch of shaders without the need
	 * to modify the shaders when you change the data contained in the UniformBufferObject.
	 * Thus you can greatly reduce the amount of API-Calls for assigning data to uniform variables.
	 * @created:	2014/12/03
	 * @author Andy Reimann <a.reimann@moorlands-grove.de>
	 */
	class UniformBufferObject : public G2::NonCopyable
	{
		friend class Shader;
		public:
			/** This constructs a new UniformBufferObject.
			 * @param shadingLanguage The shading language the UBO targets. This variable might disappear in future.
			 * @param byteSize The size in bytes the UniformBufferObject should allocate.
			 * @param data A pointer to some initial data or nullptr if no data should be used for initialization.
			 * @param bufferUsage The usage flag to use for the buffer.
			 * @param uniformBufferBindingPoint The binding point to bind the UniformBufferObject to.
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
			/** Binds the UniformBufferObject to the gfx layer for manipulation purpose.
			 */
			void bind();
			/** Updates the data inside the UniformBufferObject.
			 * @param byteOffset The offset in bytes were you want to start writing the new data.
			 * @param data The number of indices to draw or -1 = default if you just want to draw all indices.
			 * @note Make sure to bind a UniformBufferObject before calling the updateSubData function.
			 */
			void updateSubData(unsigned int byteOffset, unsigned int byteSize, void* data);
			/** Unbinds the UniformBufferObject from the gfx layer for manipulation purpose.
			 */
			void unbind();
		private:
			UniformBufferObject(UniformBufferObject const& rhs) {}
			UniformBufferObject& operator=(UniformBufferObject const& rhs) {}
			
			G2Core::GfxResource*				mUniformBufferResource;	// The resource handle in the GfxContext
			unsigned int						mNumBytes;				// The number of bytes the UniformBufferObject manages
			G2Core::BufferUsage::Name			mBufferUsage;			// The buffer usage flag
	};
};