#include "HandlerBase.h"
#include "GEARStudioEvents.h"

#include <G2Core/ECSManager.h>

using namespace G2S;

HandlerBase::HandlerBase(G2S::HandleMode::Name responsibleMode) :
	mHandleMode(responsibleMode),
	mActivated(false)
{
	// register to Editor events
	G2S::activateHandle.hook(this, &HandlerBase::_tryActivate);
}

HandlerBase::~HandlerBase()
{
	// unhook from Editor events
	G2S::activateHandle.unHookAll(this);
}

void HandlerBase::_tryActivate(G2S::HandleMode::Name mode)
{
	bool actTmp = mActivated;
	mActivated = (mode == mHandleMode);
	if (mActivated != actTmp)
	{
		// trigger update in case activation flag changed
		handleActivityChanged();
	}
}