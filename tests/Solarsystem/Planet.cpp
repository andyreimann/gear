#include "Planet.h"

Planet::Planet(
	std::string const& name,
	std::shared_ptr<G2::FBXMesh> planetMesh,
	glm::vec3 const& position,
	glm::vec3 const& scale,
	glm::vec3 const& rotationAxis,
	double period,
	double axisRotationSpeed,
	std::shared_ptr<G2::Texture> diffuseTex,
	std::shared_ptr<G2::Effect> effect) :
	mName(name),
	mPlanetMesh(planetMesh),
	mRotationAxis(rotationAxis),
	mAnchorRotationSpeed(360.0 / period),
	mAxisRotationSpeed(axisRotationSpeed),
	mSpeed(1.0)
{
	G2::EventDistributer::onRenderFrame.hook(this, &Planet::_onRenderFrame);
	G2::EventDistributer::onKeyDown.hook(this, &Planet::_onKeyDown);

	// set transformation
	mAnchor.addComponent<G2::TransformComponent>();

	auto* transformation = mPlanetMesh->addComponent<G2::TransformComponent>();
	transformation->setPosition(position);
	transformation->setScale(scale);

	// connect moon to sun with his anker - 
	// NOTE: Get the TransformComponent ptr of anchor again, because there is no
	// guarantee that it's still valid after adding a new TransformComponent!
	transformation->setParent(mAnchor.getComponent<G2::TransformComponent>());

	// set diffuse texture
	auto* renderComp = mPlanetMesh->getComponent<G2::RenderComponent>();
	renderComp->material.setTexture(G2::Sampler::DIFFUSE,diffuseTex);
	renderComp->material.setAmbient(glm::vec4(0.f, 0.f, 0.f, 1.f));

	// set planet shader
	renderComp->setEffect(effect);

}

Planet::~Planet()
{
	G2::EventDistributer::onRenderFrame.unHookAll(this);
}

void
Planet::_onRenderFrame(G2::FrameInfo const& frameInfo)
{
	// rotate around anchor
	auto* transformation = mAnchor.getComponent<G2::TransformComponent>();
	transformation->rotateY(mAnchorRotationSpeed * frameInfo.timeSinceLastFrame * mSpeed);

	// rotate around it's own axis
	transformation = mPlanetMesh->getComponent<G2::TransformComponent>();
	transformation->rotateAxis(mAxisRotationSpeed * frameInfo.timeSinceLastFrame * mSpeed, mRotationAxis);
}

void
Planet::_onKeyDown(G2::KeyCode keyCode)
{
	if (keyCode == G2::KC_P)
	{
		mSpeed = 1.0 - mSpeed;
	}
}