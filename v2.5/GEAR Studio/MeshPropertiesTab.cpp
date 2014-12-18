#include "MeshPropertiesTab.h"
#include "GEARStudioEvents.h"

#include <G2/RenderComponent.h>

#include <QtWidgets/QFileDialog>


MeshPropertiesTab::MeshPropertiesTab(QWidget *parent /*= 0*/)
	: QWidget(parent)
{
	ui.setupUi(this);
	ui.tabToggle->setText("Mesh");

	connect(ui.tabToggle, SIGNAL(clicked()), this, SLOT(toggleTab()));
	connect(ui.selectMesh, SIGNAL(clicked()), this, SLOT(selectMesh()));

	GEARStudioEvents::onManagedEntitySelected.hook(this, &MeshPropertiesTab::_init);
}

MeshPropertiesTab::~MeshPropertiesTab()
{
	GEARStudioEvents::onManagedEntitySelected.unHookAll(this);
}

void
MeshPropertiesTab::toggleTab()
{
	if (mOpen)
	{
		ui.groupBox->hide();
	}
	else
	{
		ui.groupBox->show();
	}
	mOpen = !mOpen;
}

void
MeshPropertiesTab::selectMesh()
{
	QString directory = QFileDialog::getOpenFileName(this, "Select mesh file", "", "FBX (*.fbx);MD5 (*.md5)");
}

void
MeshPropertiesTab::_init(ManagedEntity* entity)
{
	if (entity == nullptr || !entity->hasComponent<G2::RenderComponent>())
	{
		hide();
	}
	else
	{
		show();
		ui.meshPath->setText("");
	}
}