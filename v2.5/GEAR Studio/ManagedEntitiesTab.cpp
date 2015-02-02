#include "ManagedEntitiesTab.h"
#include "GEARStudioEvents.h"
#include "Scene.h"
#include "ManagedEntityPanel.h"

ManagedEntitiesTab::ManagedEntitiesTab(QWidget *parent /*= 0*/)
	: QWidget(parent)
{
	ui.setupUi(this);

	connect(ui.tableWidget->selectionModel(), SIGNAL(selectionChanged(QItemSelection const&, QItemSelection const&)), this, SLOT(itemSelected(QItemSelection const&, QItemSelection const&)));

	G2S::onSceneLoaded.hook(this, &ManagedEntitiesTab::_onSceneLoaded);
	G2S::onSceneUnloaded.hook(this, &ManagedEntitiesTab::_onSceneUnloaded);
	G2S::onManagedEntityCreated.hook(this, &ManagedEntitiesTab::_onManagedEntityCreated);
	G2S::onManagedEntitySelected.hook(this, &ManagedEntitiesTab::_onManagedEntitySelected);
	G2S::onManagedEntityRemoved.hook(this, &ManagedEntitiesTab::_onManagedEntityRemoved);

	ui.tableWidget->verticalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
}

ManagedEntitiesTab::~ManagedEntitiesTab()
{
	G2S::onSceneUnloaded.unHookAll(this);
	G2S::onSceneLoaded.unHookAll(this);
	G2S::onManagedEntityCreated.unHookAll(this);
	G2S::onManagedEntitySelected.unHookAll(this);
	G2S::onManagedEntityRemoved.unHookAll(this);
}

void ManagedEntitiesTab::_onManagedEntityCreated(Scene* scene, ManagedEntity* entity)
{
	int row = ui.tableWidget->rowCount();
	ui.tableWidget->insertRow(row);

	ui.tableWidget->setItem(row, 0, new QTableWidgetItem(std::to_string(entity->getId()).c_str()));

	ManagedEntityPanel* w = new ManagedEntityPanel(scene, entity->getId(), this);

	ui.tableWidget->setCellWidget(row, 1, w);
}

void ManagedEntitiesTab::_onManagedEntitySelected(ManagedEntity* entity)
{
	// search and check by entityId
	for (int i = 0; i < ui.tableWidget->rowCount(); ++i)
	{
		if (ui.tableWidget->item(i, 0)->text().toInt() == entity->getId())
		{
			QModelIndex index = ui.tableWidget->model()->index(i, 0);
			ui.tableWidget->selectionModel()->select(index, QItemSelectionModel::ClearAndSelect);
			index = ui.tableWidget->model()->index(i, 1);
			ui.tableWidget->selectionModel()->select(index, QItemSelectionModel::Select);
			return; // early terminate - can only be contained once
		}
	}
}

void ManagedEntitiesTab::_onManagedEntityRemoved(Scene* scene, ManagedEntity* entity)
{
	// search and check by entityId
	for (int i = 0; i < ui.tableWidget->rowCount(); ++i)
	{
		if (ui.tableWidget->item(i, 0)->text().toInt() == entity->getId())
		{
			ui.tableWidget->removeRow(i);
			return; // early terminate - can only be contained once
		}
	}
}

void
ManagedEntitiesTab::_onSceneUnloaded(Scene* scene)
{
	while (ui.tableWidget->rowCount() > 0)
	{
		ui.tableWidget->removeRow(0);
	}
}

void
ManagedEntitiesTab::_onSceneLoaded(Scene* scene)
{
	mScene = scene;
}

void ManagedEntitiesTab::itemSelected(QItemSelection const& selected, QItemSelection const& deselected)
{
	if (mScene == nullptr)
	{
		return;
	}
	auto* selectedItem = ui.tableWidget->item(ui.tableWidget->currentRow(), 0);
	if (selectedItem != nullptr)
	{
		int entityId = selectedItem->text().toInt();
		auto* managedEntity = mScene->get(entityId);
		if (managedEntity != nullptr)
		{
			G2S::onManagedEntitySelected.unHookAll(this);
			G2S::onManagedEntitySelected(managedEntity);
			G2S::onManagedEntitySelected.hook(this, &ManagedEntitiesTab::_onManagedEntitySelected);
		}
	}
}

