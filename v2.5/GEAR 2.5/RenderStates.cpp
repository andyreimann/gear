#include "RenderStates.h"
#include <utility>

using namespace G2;

RenderStates::RenderStates() 
	: mFaceCulling(FaceCulling::BACK_FACE),
	mPolygonDrawMode(PolygonDrawMode::FILL),
	mPolygonOffsetFactor(0.f),
	mPolygonOffsetUnits(0.f)
{
}

RenderStates::RenderStates(RenderStates const& rhs) 
{
	// eliminates redundant code
	*this = rhs;
}

RenderStates::RenderStates(RenderStates && rhs) 
	: mFaceCulling(FaceCulling::BACK_FACE),
	mPolygonDrawMode(PolygonDrawMode::FILL),
	mPolygonOffsetFactor(0.f),
	mPolygonOffsetUnits(0.f)
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
	// 3. Stage: modify src to a well defined state
	rhs.mFaceCulling = FaceCulling::BACK_FACE;
	rhs.mPolygonDrawMode = PolygonDrawMode::FILL;
	rhs.mPolygonOffsetFactor = 0.f;
	rhs.mPolygonOffsetUnits = 0.f;
	return *this;
}

bool
RenderStates::operator==(RenderStates const& rhs) 
{
	return 
		mFaceCulling == rhs.mFaceCulling &&
		mPolygonDrawMode == rhs.mPolygonDrawMode &&
		mPolygonOffsetFactor == rhs.mPolygonOffsetFactor &&
		mPolygonOffsetUnits == rhs.mPolygonOffsetUnits;
}
