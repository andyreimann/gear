#include "RenderTarget.h"
#include "Defines.h"
#include "Texture.h"
#include "RenderComponent.h"

#include <G2Core/ECSManager.h>
#include <G2Core/GfxDevice.h>

using namespace G2;

const G2Core::TextureFormat::Name CubeMapAttachmentPoints[6] = {
	G2Core::TextureFormat::CUBE_MAP_POS_X,
	G2Core::TextureFormat::CUBE_MAP_NEG_X,
	G2Core::TextureFormat::CUBE_MAP_POS_Y,
	G2Core::TextureFormat::CUBE_MAP_NEG_Y,
	G2Core::TextureFormat::CUBE_MAP_POS_Z,
	G2Core::TextureFormat::CUBE_MAP_NEG_Z
};

RenderTarget::RenderTarget(
	Sampler::Name targetSampler, 
	std::shared_ptr<Texture> const& renderTexture, 
	RenderTargetType::Name renderTargetType, 
	std::unordered_map<std::string,Setting>& settings)
	: mRenderTextureSampler(targetSampler), 
	mRenderTexture(renderTexture),
	mRenderTargetType(renderTargetType),
	mFrameBuffer(renderTexture->getWidth(),renderTexture->getHeight(), renderTexture->getDataFormat()),
	mUseClearColor(false),
	mRenderSystem(ECSManager::getShared().getSystem<RenderSystem,RenderComponent>())
{
	Setting const& clearColorSetting = Setting::get("ClearColor", settings, "DEFAULT");
	if(clearColorSetting.value != "DEFAULT")
	{
		mUseClearColor = true;
		mClearColor = clearColorSetting.toVec4();
	}
	// Get the buffer attachment point depending
	// on the requested output format
	mRenderTargetAttachmentPoint = G2Core::FrameBufferAttachmentPoint::getByDataFormat(
		Setting::get("OutputFormat", settings, "RGB").value
	);

	mFrameBuffer.bind();
	if(mRenderTargetType == RenderTargetType::RT_CUBE)
	{
		// just attach once to check the FBO, although we have to re-attach in each iteration for cube maps
		mFrameBuffer.attachTexture(mRenderTexture, mRenderTargetAttachmentPoint, CubeMapAttachmentPoints[0]);
	}
	else
	{

		mFrameBuffer.attachTexture(mRenderTexture, mRenderTargetAttachmentPoint, mRenderTexture->getType(), 0, 0);
	}
	mFrameBuffer.unbind();
}

RenderTarget::RenderTarget(RenderTarget && rhs) 
{
	// eliminates redundant code
	*this = std::move(rhs); // rvalue property is kept with std::move!
}

RenderTarget& RenderTarget::operator=(RenderTarget && rhs)
{
	// 1. Stage: delete maybe allocated resources on target type
	// nothing here
	// 2. Stage: transfer data from src to target
	mRenderTextureSampler = rhs.mRenderTextureSampler;
	mRenderTexture = std::move(rhs.mRenderTexture);
	mRenderTargetType = rhs.mRenderTargetType;
	mFrameBuffer = std::move(rhs.mFrameBuffer);
	mRenderTargetAttachmentPoint = rhs.mRenderTargetAttachmentPoint;
	mClearColor = rhs.mClearColor;
	mUseClearColor = rhs.mUseClearColor;
	mRenderSystem = rhs.mRenderSystem;
	// 3. Stage: modify src to a well defined state
	rhs.mRenderTextureSampler = Sampler::SAMPLER_INVALID;
	rhs.mRenderTargetType = RenderTargetType::RT_INVALID;
	rhs.mRenderTargetAttachmentPoint = G2Core::FrameBufferAttachmentPoint::ATTACHMENT_INVALID;
	return *this;
}

void
RenderTarget::bind(int renderIterationIndex) const
{
	
	mFrameBuffer.bind();
	if(mRenderTargetType == RenderTargetType::RT_CUBE)
	{
		// attach current cube map face
		mFrameBuffer.attachTexture(mRenderTexture, mRenderTargetAttachmentPoint, CubeMapAttachmentPoints[renderIterationIndex]);
	}
	else if(mRenderTargetType == RenderTargetType::RT_2D_ARRAY)
	{
		mFrameBuffer.attachTexture(mRenderTexture, mRenderTargetAttachmentPoint, mRenderTexture->mType, 0, renderIterationIndex);
	}
	
	if(mUseClearColor)
	{
		G2_gfxDevice()->clearColor(mClearColor);
	}
	else
	{
		G2_gfxDevice()->clearColor(mRenderSystem->getClearColor());
	}
	// TODO maybe later we have to pass a valid GfxResource here for the rendertarget!
	G2_gfxDevice()->clearBuffers(G2Core::Buffer::COLOR | G2Core::Buffer::DEPTH, mFrameBuffer.mRenderTargetResource);
}

void
RenderTarget::unbind() const
{
	mFrameBuffer.unbind();
}

RenderTargetType::Name
RenderTargetType::getRenderTargetType(std::string const& name) 
{
	if(name == "TEXTURE_1D") 
	{
		return RT_1D;
	}
	else if(name == "TEXTURE_2D" || name == "TEXTURE") 
	{
		return RT_2D;
	}
	else if(name == "TEXTURE_CUBE" || name == "CUBEMAP") 
	{
		return RT_CUBE;
	}
	else if(name == "TEXTURE_ARRAY" || name == "TEXTURE_2D_ARRAY") 
	{
		return RT_2D_ARRAY;
	}
	return RT_INVALID;
}