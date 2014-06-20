#include "ZSorter.h"
#include "RenderComponent.h"

using namespace G2;


bool
ZSorter::operator()(std::pair<unsigned int,VaoIndexDrawCallPair> a, std::pair<unsigned int,VaoIndexDrawCallPair> b) 
{
	float distA = glm::length(mCameraPosition - mRenderSystem->get(a.first)->worldSpaceAABBs[a.second.drawCall].getCenter());
	float distB = glm::length(mCameraPosition - mRenderSystem->get(b.first)->worldSpaceAABBs[b.second.drawCall].getCenter());
	// objects more far away are considered to be "smaller" aka rendered earlier
	return distA > distB;
}