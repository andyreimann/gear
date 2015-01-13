#include "ComponentListItemModel.h"

ComponentListItemModel::ComponentListItemModel(QObject *parent) 
	: QAbstractListModel(parent)
{
	/**
	 * REMEMBER:
	 * ListViews are only designed to work with one column!
	 */
	G2S::onSceneUnloaded.hook(this, &ComponentListItemModel::_onSceneUnloaded);
	G2S::onManagedEntityCreated.hook(this, &ComponentListItemModel::_onManagedEntityCreated);
	G2S::onManagedEntityRemoved.hook(this, &ComponentListItemModel::_onManagedEntityRemoved);
}


ComponentListItemModel::~ComponentListItemModel() 
{
	G2S::onSceneUnloaded.unHookAll(this);
	G2S::onManagedEntityCreated.unHookAll(this);
	G2S::onManagedEntityRemoved.unHookAll(this);
}

void ComponentListItemModel::_onManagedEntityCreated(Scene* scene, ManagedEntity* entity)
{
	// Create new row for that EntityId since it is not yet existing and set the data for it.
	insertRows(rowCount(), 1);
	mComponents[rowCount() - 1] = entity; // set the data on the newly added row
	//setData(index(rowCount() - 1, 0), QVariant(entity->getName().c_str()), entity);
}

void ComponentListItemModel::_onManagedEntityRemoved(Scene* scene, ManagedEntity* entity)
{
	// search and check by entityId
	for (int i = 0; i < rowCount(); ++i)
	{
		if (mComponents[i]->getId() == entity->getId())
		{
			this->removeRows(i,1);
			return; // early terminate - can only be contained once
		}
	}
}

void
ComponentListItemModel::_onSceneUnloaded(Scene* scene)
{
	// clean up entire list
	this->removeRows(0, rowCount());
}

int ComponentListItemModel::rowCount(QModelIndex const& parent /*= QModelIndex()*/) const
{
	return mComponents.size();
}

QVariant ComponentListItemModel::data(QModelIndex const& index, int role /*= Qt::DisplayRole*/) const
{
	if (!index.isValid())
	{
		return QVariant();
	}
	if (role == Qt::DisplayRole)
	{
		return QVariant(mComponents.at(index.row())->getName().c_str());
	}
	return QVariant();
}

bool ComponentListItemModel::setData(const QModelIndex &index, QVariant const& value, ManagedEntity* entity, int role)
{
	if (index.isValid() && role == Qt::EditRole)
	{
		int row = index.row();

		if (row == 0)
		{
			mComponents[row] = entity; // this will replace the pointer at that position - initially all pointers are nullptr
		}
		emit(dataChanged(index, index));

		return true;
	}

	return false;
}

bool
ComponentListItemModel::insertRows(int row, int count, QModelIndex const& parent /*= QModelIndex()*/)
{
	beginInsertRows(parent, row, row + count - 1);

	for (int r = 0; r < count; ++r)
	{
		mComponents.insert(row, nullptr);
	}

	endInsertRows();
	return true;
}

bool
ComponentListItemModel::removeRows(int row, int count, QModelIndex const& parent /*= QModelIndex()*/)
{
	beginRemoveRows(parent, row, row + count - 1);

	for (int r = 0; r < count; ++r)
	{
		mComponents.removeAt(row);
	}
	endRemoveRows();
	return true;
}

ManagedEntity*
ComponentListItemModel::entity(QModelIndex const& index)
{
	if (!index.isValid())
	{
		return nullptr;
	}
	return mComponents.at(index.row());
}
// SAMPLE OF HOW TO SEARCH FOR AN ELEMENT AND MODIFY IT
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