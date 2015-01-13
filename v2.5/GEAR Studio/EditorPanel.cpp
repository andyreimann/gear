#include "EditorPanel.h"
#include "GEARStudioEvents.h"
#include "GEAREditor.h"
#include "Defines.h"

EditorPanel::EditorPanel(GEAREditor* editor, QWidget *parent)
	: QWidget(parent),
	mEntity(nullptr),
	mEditor(editor)
{
	ui.setupUi(this);

	connect(ui.newProjectBtn, SIGNAL(clicked()), mEditor, SLOT(newProject()));
	connect(ui.openProjectBtn, SIGNAL(clicked()), mEditor, SLOT(openProject()));
	connect(ui.renderSolidBtn, SIGNAL(clicked()), this, SLOT(renderSolid()));
	connect(ui.renderWireBtn, SIGNAL(clicked()), this, SLOT(renderWire()));
	connect(ui.renderPointBtn, SIGNAL(clicked()), this, SLOT(renderPoint()));
	connect(ui.playBtn, SIGNAL(clicked()), mEditor, SLOT(exportAndStartProject()));

	G2S::onManagedEntitySelected.hook(this, &EditorPanel::_onManagedEntitySelected);
}


EditorPanel::~EditorPanel()
{
	G2S::onManagedEntitySelected.unHookAll(this);
}

void
EditorPanel::_initUi()
{
	if (mEntity == nullptr)
	{
		GEAR_QTNOEVENT(ui.renderSolidBtn, ui.renderSolidBtn->setChecked(true));
	}
	else
	{
		auto* rc = mEntity->getComponent<G2::RenderComponent>();
		if (rc != nullptr)
		{
			switch (rc->getPolygonDrawMode())
			{
				case G2Core::PolygonDrawMode::LINE:
					GEAR_QTNOEVENT(ui.renderWireBtn, ui.renderWireBtn->setChecked(true));
					break;
				case G2Core::PolygonDrawMode::POINT:
					GEAR_QTNOEVENT(ui.renderPointBtn, ui.renderPointBtn->setChecked(true));
					break;
				case G2Core::PolygonDrawMode::FILL:
				default:
					GEAR_QTNOEVENT(ui.renderSolidBtn, ui.renderSolidBtn->setChecked(true));
			}
		}
		else
		{
			GEAR_QTNOEVENT(ui.renderSolidBtn, ui.renderSolidBtn->setChecked(true));
		}
	}
}

void EditorPanel::_onManagedEntitySelected(ManagedEntity* entity)
{
	mEntity = entity;
	_initUi();
}

void
EditorPanel::renderSolid()
{
	_changePolygonDrawMode(G2Core::PolygonDrawMode::FILL);
}

void
EditorPanel::renderWire()
{
	_changePolygonDrawMode(G2Core::PolygonDrawMode::LINE);
}

void
EditorPanel::renderPoint()
{
	_changePolygonDrawMode(G2Core::PolygonDrawMode::POINT);
}

void
EditorPanel::_changePolygonDrawMode(G2Core::PolygonDrawMode::Name drawMode)
{
	if (mEntity == nullptr || !mEntity->hasComponent<G2::RenderComponent>())
	{
		return;
	}
	auto* rc = mEntity->getComponent<G2::RenderComponent>();
	rc->setPolygonDrawMode(drawMode);
}