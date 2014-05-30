#include "Pass.h"
#include "AbstractShaderPart.h"
#include "Shader.h"
#include "Texture2D.h"
#include "TextureCube.h"

using namespace G2;

Pass::Pass(
	std::vector<std::shared_ptr<Shader>> const& shaderPermutations, 
	std::unordered_map<std::string,Setting> const& settings, 
	Sampler::Name renderTargetSampler,
	RenderTargetType::Name renderTargetType) 
	: mSettings(settings),
	mShaderPermutations(shaderPermutations),
	mRenderTarget(renderTargetSampler,
				// TODO implement case RT_1D here as well!
				  renderTargetType == RenderTargetType::RT_2D ?
				  std::shared_ptr<Texture>(new Texture2D(
						NEAREST, 
						NEAREST, 
						(unsigned)Setting::get("RenderTargetWidth", mSettings, "512").toInt(), 
						(unsigned)Setting::get("RenderTargetHeight", mSettings, "512").toInt(), 
						Texture::getFormatByString(Setting::get("OutputFormat", mSettings, "RGB").value), 
						false)) : (
				  std::shared_ptr<Texture>(new TextureCube(
						NEAREST, 
						NEAREST, 
						(unsigned)Setting::get("RenderTargetWidth", mSettings, "512").toInt(), 
						(unsigned)Setting::get("RenderTargetHeight", mSettings, "512").toInt(), 
						Texture::getFormatByString(Setting::get("OutputFormat", mSettings, "RGB").value), 
						false))
				),
				renderTargetType,
				mSettings),
	mPov(PointOfView::getPointOfView(Setting::get("PointOfView", mSettings, "MAIN_CAMERA").value)),
	mZNear(Setting::get("ZNear", mSettings, "0.01").toFloat()),
	mZFar(Setting::get("ZFar", mSettings, "100.0").toFloat()),
	mPolygonOffsetFactor(Setting::get("PolygonOffsetFactor", mSettings, "1.0").toFloat()),
	mPolygonOffsetUnits(Setting::get("PolygonOffsetUnits", mSettings, "100.0").toFloat()),
	mFovY(Setting::get("FovY", mSettings, "90.0").toFloat()),
	mFlipYLevel(Setting::get("FlipYLevel", mSettings, "0.0").toFloat())
{
	if(renderTargetType == RenderTargetType::RT_CUBE)
	{
		mNumRenderIterations = 6;
	}
	else
	{
		mNumRenderIterations = 1;
	}
}

Pass::Pass(Pass && rhs)
{
	// eliminates redundant code
	*this = std::move(rhs); // rvalue property is kept with std::move!
}

Pass& Pass::operator=(Pass && rhs)
{
	// 1. Stage: delete maybe allocated resources on target type
	// nothing here
	// 2. Stage: transfer data from src to target
	mSettings = std::move(rhs.mSettings);
	mShaderPermutations = std::move(rhs.mShaderPermutations);
	mRenderTarget = std::move(rhs.mRenderTarget);
	mNumRenderIterations = rhs.mNumRenderIterations;
	mPov = rhs.mPov;
	mZNear = rhs.mZNear;
	mZFar = rhs.mZFar;
	mFovY = rhs.mFovY;
	mPolygonOffsetFactor = rhs.mPolygonOffsetFactor;
	mPolygonOffsetUnits = rhs.mPolygonOffsetUnits;
	mFlipYLevel = rhs.mFlipYLevel;
	// 3. Stage: modify src to a well defined state
	rhs.mNumRenderIterations = 0;
	rhs.mPov = PointOfView::POV_INVALID;
	return *this;
}

std::shared_ptr<Shader>
Pass::getShader(Material const& material, VertexArrayObject const& vao) const
{
	#undef max
	int bestFitFailCount = std::numeric_limits<int>::max();
	std::shared_ptr<Shader> bestFit = std::shared_ptr<Shader>();
	int num = (int)mShaderPermutations.size();
	for (int i = 0; i < num ; ++i) 
	{
		auto const& conditions = mShaderPermutations[i]->getConditions();
		int numConditions = (int)conditions.size();
		int failCount = 0; // number of checks failed for this Shader
		for (int k = 0; k < numConditions ; ++k) 
		{
			if(!conditions[k].check(material,vao))
			{
				++failCount;
			}
		}
		if(failCount == 0)
		{
			// best fit - all conditions met
			return mShaderPermutations[i];
		}
		else if(failCount < bestFitFailCount)
		{
			// check if less condition checks failed
			bestFitFailCount = failCount;
			bestFit = mShaderPermutations[i];
		}
	}
	return std::shared_ptr<Shader>();
}

void 
Pass::preRender() const
{
	// called from RenderSystem when rendering the pass 
	GLDEBUG( glEnable(GL_POLYGON_OFFSET_FILL) );
	GLDEBUG( glPolygonOffset( mPolygonOffsetFactor, mPolygonOffsetUnits ) );
	GLDEBUG( glViewport(0,0,mRenderTarget.getRenderTexture()->getWidth(),mRenderTarget.getRenderTexture()->getHeight()));
}

void 
Pass::postRender() const
{
	GLDEBUG( glDisable(GL_POLYGON_OFFSET_FILL) );
}

void
Pass::Builder::addVertexShaderParts(std::vector<std::shared_ptr<AbstractShaderPart>> const& parts) 
{
	for(auto it = parts.begin(); it != parts.end(); ++it)
	{
		vertexShaderParts.push_back(*it);
	}
}

void
Pass::Builder::addFragmentShaderParts(std::vector<std::shared_ptr<AbstractShaderPart>> const& parts) 
{
	for(auto it = parts.begin(); it != parts.end(); ++it)
	{
		fragmentShaderParts.push_back(*it);
	}
}

PointOfView::Name
PointOfView::getPointOfView(std::string const& name) 
{
	if(name == "MAIN_CAMERA") 
	{
		return MAIN_CAMERA;
	}
	if(name == "MAIN_CAMERA_FLIP_Y") 
	{
		return MAIN_CAMERA_FLIP_Y;
	}
	else if(name == "LOCAL") 
	{
		return LOCAL;
	}
	return POV_INVALID;
}