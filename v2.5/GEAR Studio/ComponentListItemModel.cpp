#include "ComponentListItemModel.h"

#include <G2Core/ECSManager.h>
#include <G2/NameComponent.h>

ComponentListItemModel::ComponentListItemModel(QObject *parent) 
	: QAbstractListModel(parent)
{
	/**
	 * REMEMBER:
	 * ListViews are only designed to work with one column!
	 */
	GEARStudioEvents::onSceneUnloaded.hook(this, &ComponentListItemModel::_onSceneUnloaded);
	GEARStudioEvents::onManagedEntityCreated.hook(this, &ComponentListItemModel::_onManagedEntityCreated);
}


ComponentListItemModel::~ComponentListItemModel() 
{
	GEARStudioEvents::onSceneUnloaded.unHookAll(this);
	GEARStudioEvents::onManagedEntityCreated.unHookAll(this);
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
ComponentListItemModel::setData(const QModelIndex &index, QVariant const& value, unsigned int entityId, int role)
{
	if(index.isValid() && role == Qt::EditRole)
	{
		int row = index.row();
 
		if(row == 0)
		{
			ComponentState& comp = mComponents[row];
			comp.name = value.toString().toStdString();
			comp.entityId = entityId;
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
		mComponents.insert(row, ComponentState());
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

void ComponentListItemModel::_onManagedEntityCreated(Scene* scene, ManagedEntity* entity)
{
	// search for an already existing row for that EntityId
	//for (int i = 0; i < rowCount(); ++i)
	//{
	//	if (mComponents[i].entityId == entityId)
	//	{
	//		mComponents[i].types |= type;
	//		mComponents[i].name = _buildDisplayName(entityId, mComponents[i].types);

	//		QModelIndex idx = index(i, 0);
	//		emit(dataChanged(idx, idx));
	//		return;
	//	}
	//}
	// Create new row for that EntityId since it is not yet existing
	insertRows(rowCount(), 1);

	setData(index(rowCount() - 1, 0), QVariant(_buildDisplayName(entity).c_str()), entity->getId());
}

std::string
ComponentListItemModel::_buildDisplayName(ManagedEntity* entity) const
{
	return entity->getName();
}

void
ComponentListItemModel::_onSceneUnloaded(Scene* scene)
{
	// clean up entire list
	this->removeRows(0, rowCount());
}