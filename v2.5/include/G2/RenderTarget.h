#pragma once
#include "Texture2D.h"
#include "FrameBuffer.h"
#include "Sampler.h"

namespace G2 
{
	/// This class holds the data used for a RenderTarget.
	/// RenderTargets can be used for rendering to them.
	/// @created:	2014/05/07
	/// @author Andy Reimann <a.reimann@moorlands-grove.de>
	class RenderTarget 
	{
		friend class Pass;
		public:
			/// This constructs a new RenderTarget with the given with and height
			RenderTarget(Sampler::Name targetSampler, unsigned width, unsigned height);
			
			/// Move ctor.
			RenderTarget(RenderTarget && rhs);
			/// Move ctor.
			RenderTarget& operator=(RenderTarget && rhs);
			/// Binds the RenderTarget to  be able to render to it.
			void bind() const;
			/// Unbinds the RenderTarget to disable rendering to it.
			void unbind() const;
			/// This function will return the RenderTextureSampler. 
			/// @return The current RenderTextureSampler.
			Sampler::Name const& getRenderTextureSampler() const { return mRenderTextureSampler; }
			///  This function will return the RenderTexture. 
			/// @return The current RenderTexture.
			G2::Texture2D const& getRenderTexture() const { return mRenderTexture; }
		private:
			RenderTarget() {}

			Sampler::Name mRenderTextureSampler;	// The sampler, the RenderTexture should be bound to
			Texture2D mRenderTexture;				// The texture to render to
			FrameBuffer mFrameBuffer;				// The framebuffer, the render texture is attached to
	};
};