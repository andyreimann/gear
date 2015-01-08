#include "RenderStates.h"
#include <utility>

#include <G2Core/GfxDevice.h>

using namespace G2;

RenderStates::RenderStates() 
	: mFaceCulling(G2Core::FaceCulling::BACK_FACE),
	mPolygonDrawMode(G2Core::PolygonDrawMode::FILL),
	mPolygonOffsetFactor(0.f),
	mPolygonOffsetUnits(0.f),
	mSourceFactor(G2Core::BlendFactor::SRC_ALPHA),
	mDestinationFactor(G2Core::BlendFactor::ONE_MINUS_SRC_ALPHA),
	mRenderDepth(true),
	mDepthFunc(G2Core::DepthFunc::LESS)
{
	
}

RenderStates::RenderStates(RenderStates const& rhs) 
{
	// eliminates redundant code
	*this = rhs;
}

RenderStates::RenderStates(RenderStates && rhs) 
	: mFaceCulling(G2Core::FaceCulling::BACK_FACE),
	mPolygonDrawMode(G2Core::PolygonDrawMode::FILL),
	mPolygonOffsetFactor(0.f),
	mPolygonOffsetUnits(0.f),
	mSourceFactor(G2Core::BlendFactor::SRC_ALPHA),
	mDestinationFactor(G2Core::BlendFactor::ONE_MINUS_SRC_ALPHA),
	mRenderDepth(true),
	mDepthFunc(G2Core::DepthFunc::LESS)
{
	// eliminates redundant code
	*this = std::move(rhs); // rvalue property is kept with std::move!
}

RenderStates&
RenderStates::operator=(RenderStates const& rhs) 
{
	// do assignment here
	mFaceCulling = rhs.mFaceCulling;
	mPolygonDrawMode = rhs.mPolygonDrawMode;
	mPolygonOffsetFactor = rhs.mPolygonOffsetFactor;
	mPolygonOffsetUnits = rhs.mPolygonOffsetUnits;
	mSourceFactor = rhs.mSourceFactor;
	mDestinationFactor = rhs.mDestinationFactor;
	mRenderDepth = rhs.mRenderDepth;
	mDepthFunc = rhs.mDepthFunc;
	return *this;
}

RenderStates&
RenderStates::operator=(RenderStates && rhs) 
{
	// 1. Stage: delete maybe allocated resources on target type
	// 2. Stage: transfer data from src to target
	mFaceCulling = rhs.mFaceCulling;
	mPolygonDrawMode = rhs.mPolygonDrawMode;
	mPolygonOffsetFactor = rhs.mPolygonOffsetFactor;
	mPolygonOffsetUnits = rhs.mPolygonOffsetUnits;
	mSourceFactor = rhs.mSourceFactor;
	mDestinationFactor = rhs.mDestinationFactor;
	mRenderDepth = rhs.mRenderDepth;
	mDepthFunc = rhs.mDepthFunc;
	// 3. Stage: modify src to a well defined state
	rhs.mFaceCulling = G2Core::FaceCulling::BACK_FACE;
	rhs.mPolygonDrawMode = G2Core::PolygonDrawMode::FILL;
	rhs.mPolygonOffsetFactor = 0.f;
	rhs.mPolygonOffsetUnits = 0.f;
	rhs.mSourceFactor = G2Core::BlendFactor::SRC_ALPHA;
	rhs.mDestinationFactor = G2Core::BlendFactor::ONE_MINUS_SRC_ALPHA;
	rhs.mRenderDepth = true;
	rhs.mDepthFunc = G2Core::DepthFunc::LESS;
	return *this;
}

bool
RenderStates::operator==(RenderStates const& rhs) 
{
	return 
		mFaceCulling == rhs.mFaceCulling &&
		mPolygonDrawMode == rhs.mPolygonDrawMode &&
		mPolygonOffsetFactor == rhs.mPolygonOffsetFactor &&
		mPolygonOffsetUnits == rhs.mPolygonOffsetUnits &&
		mSourceFactor == rhs.mSourceFactor &&
		mDestinationFactor == rhs.mDestinationFactor &&
		mRenderDepth == rhs.mRenderDepth &&
		mDepthFunc == rhs.mDepthFunc;
}

void
RenderStates::applyStates(bool inPass) const
{
	G2_gfxDevice()->updateRenderStates(mFaceCulling, mPolygonDrawMode, mPolygonOffsetFactor, mPolygonOffsetUnits, mSourceFactor, mDestinationFactor, mDepthFunc);
	G2_gfxDevice()->setDepthWritesEnabled(mRenderDepth);
}