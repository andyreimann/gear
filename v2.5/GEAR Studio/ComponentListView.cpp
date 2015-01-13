#include "ComponentListView.h"
#include "ComponentListItemModel.h"
#include <iostream>

ComponentListView::ComponentListView(QWidget *parent)
	: QListView(parent)
{
}


ComponentListView::~ComponentListView(void)
{
}

void
ComponentListView::currentChanged(const QModelIndex & current, const QModelIndex & previous)
{
	auto* entity = ((ComponentListItemModel*)model())->entity(current);

	G2S::onManagedEntitySelected(entity);
}
