#include "ComponentListItemModel.h"

#include <G2Core/ECSManager.h>
#include <G2Editor/EditorComponent.h>
#include <G2/NameComponent.h>

ComponentListItemModel::ComponentListItemModel(QObject *parent) 
	: QAbstractListModel(parent)
{
	/**
	 * REMEMBER:
	 * ListViews are only designed to work with one column!
	 */
	// TODO Integrate own System for THIS Editor!
	G2::ECSManager::getShared().getSystem<G2::Editor::EditorSystem,G2::Editor::EditorComponent>()->onRenderComponentAdded.hook(this, &ComponentListItemModel::_onRenderComponentAdded);
	G2::ECSManager::getShared().getSystem<G2::Editor::EditorSystem,G2::Editor::EditorComponent>()->onRenderComponentRemoved.hook(this, &ComponentListItemModel::_onRenderComponentRemoved);
}


ComponentListItemModel::~ComponentListItemModel() 
{
	G2::ECSManager::getShared().getSystem<G2::Editor::EditorSystem,G2::Editor::EditorComponent>()->onRenderComponentAdded.unHookAll(this);
	G2::ECSManager::getShared().getSystem<G2::Editor::EditorSystem,G2::Editor::EditorComponent>()->onRenderComponentRemoved.unHookAll(this);
}

int
ComponentListItemModel::rowCount(QModelIndex const& parent /*= QModelIndex()*/) const  
{
	return mComponents.size();
}

QVariant
ComponentListItemModel::data(QModelIndex const& index, int role /*= Qt::DisplayRole*/) const  
{
	if(!index.isValid())
	{
		return QVariant();
	}
	if ( role == Qt::DisplayRole ) 
	{
		return QVariant(mComponents.at(index.row()).name.c_str());
	}
	return QVariant();
}
	 
bool 
ComponentListItemModel::setData(const QModelIndex &index, QVariant const& value, int role)
{
	if(index.isValid() && role == Qt::EditRole)
	{
		int row = index.row();
 
		if(row == 0)
		{
			Component& comp = mComponents[row];
			switch(index.column())
			{
				case 0:
					comp.name = value.toString().toStdString();
				case 1:
					comp.entityId = value.toUInt();
			}
		}
		emit(dataChanged(index, index));
 
		return true;
	}
 
	return false;
}

bool
ComponentListItemModel::insertRows(int row, int count, QModelIndex const& parent /*= QModelIndex()*/) 
{
	beginInsertRows(parent, row, row+count-1);

	for(int r = 0; r < count; ++r)
	{
		mComponents.insert(row, Component("no data yet",0));
	}

	endInsertRows();
	return true;
}

bool
ComponentListItemModel::removeRows(int row, int count, QModelIndex const& parent /*= QModelIndex()*/) 
{
	beginRemoveRows(parent, row, row+count-1);

	for(int r = 0; r < count; ++r)
	{
		mComponents.removeAt(row);
	}
	endRemoveRows();
	return true;
}

void
ComponentListItemModel::_onRenderComponentAdded(unsigned int entityId) 
{
	insertRows(rowCount(),1);
	auto* nameComponent = G2::ECSManager::getShared().getSystem<G2::NameSystem,G2::NameComponent>()->get(entityId);
	if(nameComponent != nullptr)
	{
		setData(index(rowCount()-1,0), QVariant(nameComponent->name.c_str()));
	}
	else 
	{
		setData(index(rowCount()-1,0), QVariant("G2::RenderComponent"));
	}
}

void
ComponentListItemModel::_onRenderComponentRemoved(unsigned int entityId) 
{
	for(int i = 0; i < rowCount(); ++i)
	{
		if(mComponents[i].entityId == entityId)
		{
			removeRows(i,1);
			return;
		}
	}

}