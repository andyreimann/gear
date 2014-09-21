#pragma once
#include "FrameBuffer.h"
#include "Sampler.h"
#include "Setting.h"

#include <memory>
#include <unordered_map>

#include <glm/glm.hpp>


namespace G2 
{
	class Texture;
	class RenderSystem;

	namespace RenderTargetType
	{
		enum Name {
			RT_1D,
			RT_2D,
			RT_2D_ARRAY,
			RT_CUBE,
			RT_INVALID,
		};
		/** This function will parse the given string to the appropriate
		 * RenderTargetType enum value.
		 * @param name The name to parse.
		 * @return The parsed RenderTargetType enum value.
		 */
		RenderTargetType::Name getRenderTargetType(std::string const& name);
	};
	/// This class holds the data used for a RenderTarget.
	/// RenderTargets can be used for rendering to them.
	/// @created:	2014/05/07
	/// @author Andy Reimann <a.reimann@moorlands-grove.de>
	class RenderTarget 
	{
		friend class Pass;
		public:
			/// This constructs a new RenderTarget with the given with and height
			RenderTarget(
				Sampler::Name targetSampler, 
				std::shared_ptr<Texture> const& renderTexture, 
				RenderTargetType::Name renderTargetType, 
				std::unordered_map<std::string,Setting>& settings = std::unordered_map<std::string,Setting>()
			);
			
			/// Move ctor.
			RenderTarget(RenderTarget && rhs);
			/// Move ctor.
			RenderTarget& operator=(RenderTarget && rhs);
			/// Binds the RenderTarget to  be able to render to it.
			/// @param renderIterationIndex The index describing in which iteration (-1) the 
			/// RenderTarget is currently used (e.g. cube map renderings need 6 iterations).
			void bind(int renderIterationIndex) const;
			/// Unbinds the RenderTarget to disable rendering to it.
			void unbind() const;
			/// This function will return the RenderTextureSampler. 
			/// @return The current RenderTextureSampler.
			Sampler::Name const& getRenderTextureSampler() const { return mRenderTextureSampler; }
			///  This function will return the RenderTexture. 
			/// @return The current RenderTexture.
			std::shared_ptr<Texture> const& getRenderTexture() const { return mRenderTexture; }
			/// This function will return the RenderTargetType. 
			/// @return The CURRENT RenderTargetType.
			RenderTargetType::Name const& getRenderTargetType() const { return mRenderTargetType; }
		private:
			RenderTarget() {}

			Sampler::Name				mRenderTextureSampler;	// The sampler, the RenderTexture should be bound to
			std::shared_ptr<Texture>	mRenderTexture;			// The texture to render to
			RenderTargetType::Name		mRenderTargetType;		// The type of render target
			FrameBuffer					mFrameBuffer;			// The framebuffer, the render texture is attached to
			G2Core::FrameBufferAttachmentPoint::Name mRenderTargetAttachmentPoint; // The attachment point, the Texture is attached to the FrameBuffer
			int							mNumCascades;			// The number of cascades, the render texture has
			
			glm::vec4					mClearColor;
			bool						mUseClearColor;
			RenderSystem*				mRenderSystem;
	};
};