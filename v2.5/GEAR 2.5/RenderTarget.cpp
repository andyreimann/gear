#include "RenderTarget.h"
#include "Defines.h"
#include "Texture.h"

using namespace G2;

const int CubeMapAttachmentPoints[6] = {
	G2::CUBE_MAP_POS_X,
	G2::CUBE_MAP_NEG_X,
	G2::CUBE_MAP_POS_Y,
	G2::CUBE_MAP_NEG_Y,
	G2::CUBE_MAP_POS_Z,
	G2::CUBE_MAP_NEG_Z
};

RenderTarget::RenderTarget(
	Sampler::Name targetSampler, 
	std::shared_ptr<Texture> const& renderTexture, 
	RenderTargetType::Name renderTargetType, 
	std::unordered_map<std::string,Setting>& settings)
	: mRenderTextureSampler(targetSampler), 
	mRenderTexture(renderTexture),
	mRenderTargetType(renderTargetType),
	mFrameBuffer(renderTexture->getWidth(),renderTexture->getHeight())
{
	// Get the buffer attachment point depending
	// on the requested output format
	mRenderTargetAttachmentPoint = BufferAttachment::getByOutputFormat(
		Setting::get("OutputFormat", settings, "RGB").value
	);

	if(mRenderTargetType == RenderTargetType::RT_CUBE)
	{
		// just attach once to check the FBO, although we have to re-attach in each iteration for cube maps
		mFrameBuffer.attachTexture(mRenderTexture, mRenderTargetAttachmentPoint, CubeMapAttachmentPoints[0]);
	}
	else
	{
		mFrameBuffer.attachTexture(mRenderTexture, mRenderTargetAttachmentPoint, mRenderTexture->mType);
	}
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
	// 3. Stage: modify src to a well defined state
	rhs.mRenderTextureSampler = Sampler::SAMPLER_INVALID;
	rhs.mRenderTargetType = RenderTargetType::RT_INVALID;
	rhs.mRenderTargetAttachmentPoint = BufferAttachment::ATTACHMENT_INVALID;
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

	//GLDEBUG( glClearColor(0.3f,0.0f,0.0f,1.0f) );
	GLDEBUG( glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT) );
}

void
RenderTarget::unbind() const
{
	mFrameBuffer.unbind();
}

RenderTargetType::Name
RenderTargetType::getRenderTargetType(std::string const& name) 
{
	if(name == "sampler1D") 
	{
		return RT_1D;
	}
	else if(name == "sampler2D") 
	{
		return RT_2D;
	}
	else if(name == "samplerCube") 
	{
		return RT_CUBE;
	}
	return RT_INVALID;
}