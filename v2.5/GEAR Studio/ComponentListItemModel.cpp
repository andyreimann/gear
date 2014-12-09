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
}


ComponentListItemModel::~ComponentListItemModel() 
{
	G2::ECSManager::getShared().getSystem<G2::RenderSystem, G2::RenderComponent>()
		->onComponentAddedEvent.unHookAll(this);
	G2::ECSManager::getShared().getSystem<G2::SplineAnimationSystem, G2::SplineAnimation>()
		->onComponentAddedEvent.unHookAll(this);
	G2::ECSManager::getShared().getSystem<G2::CameraSystem, G2::CameraComponent>()
		->onComponentAddedEvent.unHookAll(this);
	G2::ECSManager::getShared().getSystem<G2::TransformSystem, G2::TransformComponent>()
		->onComponentAddedEvent.unHookAll(this);

	G2::ECSManager::getShared().getSystem<G2::RenderSystem, G2::RenderComponent>()
		->onComponentRemovedEvent.unHookAll(this);
	G2::ECSManager::getShared().getSystem<G2::SplineAnimationSystem, G2::SplineAnimation>()
		->onComponentRemovedEvent.unHookAll(this);
	G2::ECSManager::getShared().getSystem<G2::CameraSystem, G2::CameraComponent>()
		->onComponentRemovedEvent.unHookAll(this);
	G2::ECSManager::getShared().getSystem<G2::TransformSystem, G2::TransformComponent>()
		->onComponentRemovedEvent.unHookAll(this);
}

void ComponentListItemModel::startListening()
{
	G2::ECSManager::getShared().getSystem<G2::RenderSystem, G2::RenderComponent>()
		->onComponentAddedEvent.hook(this, &ComponentListItemModel::_onRenderComponentAdded);
	G2::ECSManager::getShared().getSystem<G2::SplineAnimationSystem, G2::SplineAnimation>()
		->onComponentAddedEvent.hook(this, &ComponentListItemModel::_onSplineAnimationAdded);
	G2::ECSManager::getShared().getSystem<G2::CameraSystem, G2::CameraComponent>()
		->onComponentAddedEvent.hook(this, &ComponentListItemModel::_onCameraComponentAdded);
	G2::ECSManager::getShared().getSystem<G2::TransformSystem, G2::TransformComponent>()
		->onComponentAddedEvent.hook(this, &ComponentListItemModel::_onTransformComponentAdded);

	G2::ECSManager::getShared().getSystem<G2::RenderSystem, G2::RenderComponent>()
		->onComponentRemovedEvent.hook(this, &ComponentListItemModel::_onRenderComponentRemoved);
	G2::ECSManager::getShared().getSystem<G2::SplineAnimationSystem, G2::SplineAnimation>()
		->onComponentRemovedEvent.hook(this, &ComponentListItemModel::_onSplineAnimationRemoved);
	G2::ECSManager::getShared().getSystem<G2::CameraSystem, G2::CameraComponent>()
		->onComponentRemovedEvent.hook(this, &ComponentListItemModel::_onCameraComponentRemoved);
	G2::ECSManager::getShared().getSystem<G2::TransformSystem, G2::TransformComponent>()
		->onComponentRemovedEvent.hook(this, &ComponentListItemModel::_onTransformComponentRemoved);
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
ComponentListItemModel::setData(const QModelIndex &index, QVariant const& value, unsigned int entityId, G2::Studio::ComponentFlag::Name type, int role)
{
	if(index.isValid() && role == Qt::EditRole)
	{
		int row = index.row();
 
		if(row == 0)
		{
			ComponentState& comp = mComponents[row];
			comp.name = value.toString().toStdString();
			comp.entityId = entityId;
			comp.types = (int)type;
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

void
ComponentListItemModel::_onRenderComponentAdded(G2::RenderSystem* system, unsigned int entityId)
{
	_onCoreComponentAdded(entityId, G2::Studio::ComponentFlag::RENDER_COMPONENT);
}

void
ComponentListItemModel::_onSplineAnimationAdded(G2::SplineAnimationSystem* system, unsigned int entityId)
{
	_onCoreComponentAdded(entityId, G2::Studio::ComponentFlag::SPLINE_ANIMATION);
}

void
ComponentListItemModel::_onCameraComponentAdded(G2::CameraSystem* system, unsigned int entityId)
{
	_onCoreComponentAdded(entityId, G2::Studio::ComponentFlag::CAMERA_COMPONENT);
}

void
ComponentListItemModel::_onTransformComponentAdded(G2::TransformSystem* system, unsigned int entityId)
{
	_onCoreComponentAdded(entityId, G2::Studio::ComponentFlag::TRANSFORM_COMPONENT);
}

void
ComponentListItemModel::_onRenderComponentRemoved(G2::RenderSystem* system, unsigned int entityId)
{
	_onCoreComponentRemoved(entityId, G2::Studio::ComponentFlag::RENDER_COMPONENT);
}

void
ComponentListItemModel::_onSplineAnimationRemoved(G2::SplineAnimationSystem* system, unsigned int entityId)
{
	_onCoreComponentRemoved(entityId, G2::Studio::ComponentFlag::SPLINE_ANIMATION);
}

void
ComponentListItemModel::_onCameraComponentRemoved(G2::CameraSystem* system, unsigned int entityId)
{
	_onCoreComponentRemoved(entityId, G2::Studio::ComponentFlag::CAMERA_COMPONENT);
}

void
ComponentListItemModel::_onTransformComponentRemoved(G2::TransformSystem* system, unsigned int entityId)
{
	_onCoreComponentRemoved(entityId, G2::Studio::ComponentFlag::TRANSFORM_COMPONENT);
}

void ComponentListItemModel::_onCoreComponentAdded(unsigned int entityId, G2::Studio::ComponentFlag::Name type)
{
	// search for an already existing row for that EntityId
	for (int i = 0; i < rowCount(); ++i)
	{
		if (mComponents[i].entityId == entityId)
		{
			mComponents[i].types |= type;
			mComponents[i].name = _buildDisplayName(entityId, mComponents[i].types);

			QModelIndex idx = index(i, 0);
			emit(dataChanged(idx, idx));
			return;
		}
	}
	// Create new row for that EntityId since it is not yet existing
	insertRows(rowCount(), 1);

	setData(index(rowCount() - 1, 0), QVariant(_buildDisplayName(entityId, type).c_str()), entityId, type);
}

void ComponentListItemModel::_onCoreComponentRemoved(unsigned int entityId, G2::Studio::ComponentFlag::Name type)
{
	for (int i = 0; i < rowCount(); ++i)
	{
		if (mComponents[i].entityId == entityId)
		{
			mComponents[i].types &= ~type;
			if (mComponents[i].types == 0)
			{
				removeRows(i, 1);
			}
			return;
		}
	}
}

std::string
ComponentListItemModel::_buildDisplayName(unsigned int entityId, G2::Studio::ComponentMask types) const
{
	auto* nameComponent = G2::ECSManager::getShared().getSystem<G2::NameSystem, G2::NameComponent>()->get(entityId);
	std::stringstream name;
	name << "Entity ";
	if (nameComponent != nullptr)
	{
		name << "['" + nameComponent->name + "'] ";
	}

	if ((types & G2::Studio::ComponentFlag::RENDER_COMPONENT) == G2::Studio::ComponentFlag::RENDER_COMPONENT)
	{
		name << "MESH ";
	}
	if ((types & G2::Studio::ComponentFlag::SPLINE_ANIMATION) == G2::Studio::ComponentFlag::SPLINE_ANIMATION)
	{
		name << "SPL ";
	}
	if ((types & G2::Studio::ComponentFlag::CAMERA_COMPONENT) == G2::Studio::ComponentFlag::CAMERA_COMPONENT)
	{
		name << "CAM ";
	}
	if ((types & G2::Studio::ComponentFlag::TRANSFORM_COMPONENT) == G2::Studio::ComponentFlag::TRANSFORM_COMPONENT)
	{
		name << "TRANS ";
	}
	return name.str();
}