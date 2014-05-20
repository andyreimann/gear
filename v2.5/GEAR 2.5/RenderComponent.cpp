#include "RenderComponent.h"

using namespace G2;

RenderComponent::RenderComponent() 
	: billboarding(false),
	drawMode(GL_TRIANGLES)
{
}

RenderComponent::RenderComponent(unsigned int numVertexArrayObjects) 
	: billboarding(false),
	drawMode(GL_TRIANGLES)
{
	vaos.resize(numVertexArrayObjects);
}

RenderComponent::RenderComponent(RenderComponent && rhs) 
{
	// eliminates redundant code
	*this = std::move(rhs); // rvalue property is kept with std::move!
}

RenderComponent&
RenderComponent::operator=(RenderComponent && rhs) 
{
	material = std::move(rhs.material);
	vaos = std::move(rhs.vaos);
	drawMode = rhs.drawMode;
	billboarding = rhs.billboarding;
	mEffect = std::move(rhs.mEffect);
	mShaderCache = std::move(rhs.mShaderCache);

	rhs.vaos.clear();
	rhs.drawMode = GL_INVALID_VALUE;
	
	return static_cast<RenderComponent&>(BaseComponent::operator=(std::move(rhs)));
}

void
RenderComponent::setEffect(std::shared_ptr<G2::Effect> const& value) 
{
	mEffect = value; 
	mShaderCache = ShaderCache(); // invalidate
}