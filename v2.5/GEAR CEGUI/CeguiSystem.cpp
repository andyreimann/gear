#include "CeguiSystem.h"
#include "CeguiComponent.h"

#include <G2/Logger.h>
#include <CEGUI/CEGUI.h>
#include <utility>

using namespace G2::UI;

CeguiSystem::CeguiSystem() 
	: mRenderer(CEGUI::OpenGL3Renderer::bootstrapSystem())
{
	// http://static.cegui.org.uk/docs/current/rendering_tutorial.html
	// Bootstrap CEGUI::System with an OpenGL3Renderer object that uses the
	// current GL viewport, the DefaultResourceProvider, and the default
	// ImageCodec.
	//
	// NB: Your OpenGL context must already be initialised when you call this; CEGUI
	// will not create the OpenGL context itself.
}

void
CeguiSystem::runPhase(std::string const& name, G2::FrameInfo const& frameInfo) 
{ 
	if(name == "render")
	{
		G2::logger << "Render GUI" << G2::endl;
		GLDEBUG( glBindTexture(GL_TEXTURE_2D, 0) );
		GLDEBUG( glUseProgram(0) );
		GLDEBUG( glActiveTexture(GL_TEXTURE0) );
		CEGUI::System::getSingleton().renderAllGUIContexts();
	}
}

CeguiSystem::~CeguiSystem() 
{
}