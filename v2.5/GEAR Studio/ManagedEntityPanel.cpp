#include "ManagedEntityPanel.h"
#include "Scene.h"

ManagedEntityPanel::ManagedEntityPanel(Scene* currentScene, unsigned int entityId, QWidget *parent)
	: QWidget(parent),
	mScene(currentScene),
	mEntityId(entityId)
{
	ui.setupUi(this);

	auto* entity = currentScene->get(entityId);
	if (entity != nullptr)
	{
		ui.entityName->setText(entity->getName().c_str());
	}

}