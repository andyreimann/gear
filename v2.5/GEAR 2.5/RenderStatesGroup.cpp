#include "RenderStatesGroup.h"
#include <utility>

using namespace G2;

RenderStatesGroup::RenderStatesGroup() 
{
}

RenderStatesGroup::RenderStatesGroup(unsigned int entityId, RenderStates const& renderStates) 
{
	mEntityIds.push_back(entityId);
	mRenderStates = renderStates;
}

RenderStatesGroup::RenderStatesGroup(RenderStatesGroup const& rhs) 
{
	// do copy here
	*this = rhs;
}

RenderStatesGroup::RenderStatesGroup(RenderStatesGroup && rhs) 
{
	// eliminates redundant code
	*this = std::move(rhs); // rvalue property is kept with std::move!
}

RenderStatesGroup&
RenderStatesGroup::operator=(RenderStatesGroup const& rhs) 
{
	// do assignment here
	mRenderStates = rhs.mRenderStates;
	mEntityIds = rhs.mEntityIds;
	return *this;
}

RenderStatesGroup&
RenderStatesGroup::operator=(RenderStatesGroup && rhs) 
{
	// 1. Stage: delete maybe allocated resources on target type
	// 2. Stage: transfer data from src to target
	mRenderStates = std::move(rhs.mRenderStates);
	mEntityIds = std::move(rhs.mEntityIds);
	// 3. Stage: modify src to a well defined state
	mEntityIds.clear();
	return *this;
}

bool
RenderStatesGroup::consume(unsigned int entityId, RenderStates const& states) 
{
	if(mRenderStates == states)
	{
		mEntityIds.push_back(entityId);
		return true;
	}
	return false;
}

bool
RenderStatesGroup::erase(unsigned int entityId) 
{
	for(int i = 0; i < mEntityIds.size(); ++i)
	{
		if(mEntityIds[i] == entityId)
		{
			std::swap(mEntityIds[i], mEntityIds.back());
			mEntityIds.pop_back();
			return true;
		}
	}
	return false;
}