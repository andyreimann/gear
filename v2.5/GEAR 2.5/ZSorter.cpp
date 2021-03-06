#include "ZSorter.h"
#include "RenderComponent.h"

using namespace G2;


bool
ZSorter::operator()(std::pair<unsigned int,int> a, std::pair<unsigned int,int> b) 
{
	float distA = glm::length(mCameraPosition - mRenderSystem->get(a.first)->getDrawCall(a.second).getWorldSpaceAABB().getCenter());
	float distB = glm::length(mCameraPosition - mRenderSystem->get(b.first)->getDrawCall(b.second).getWorldSpaceAABB().getCenter());
	// objects more far away are considered to be "smaller" aka rendered earlier
	return distA > distB;
}