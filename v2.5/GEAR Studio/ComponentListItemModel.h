#pragma once
#include "Defines.h"
#include "GEARStudioEvents.h"

#include <G2/RenderComponent.h>
#include <G2/SplineAnimation.h>
#include <G2/CameraComponent.h>
#include <G2/TransformComponent.h>
#include <G2/RenderComponent.h>

#include <QtCore/QAbstractListModel>
#include <QtCore/QList>

/** This class acts as a model for the component list of the editor.
* The component list is a list of entries, where every entry represents a group of components
* attached to a single G2::Entity.
* @created	2014/12/09
* @author Andy Reimann <a.reimann@moorlands-grove.de>
*/
class ComponentListItemModel : public QAbstractListModel
{
	Q_OBJECT

	public:
		ComponentListItemModel(QObject *parent = 0);
		~ComponentListItemModel();

		virtual int rowCount(QModelIndex const& parent = QModelIndex()) const override;
		virtual QVariant data(QModelIndex const& index, int role = Qt::DisplayRole) const override;
		virtual bool insertRows(int row, int count, QModelIndex const& parent = QModelIndex()) override;
		virtual bool removeRows(int row, int count, QModelIndex const& parent = QModelIndex()) override;

		bool setData(const QModelIndex &index, QVariant const& value, unsigned int entityId, int role = Qt::EditRole);
	private:

		void _onSceneUnloaded(Scene* scene);
		void _onManagedEntityCreated(Scene* scene, ManagedEntity* entity);

		std::string _buildDisplayName(ManagedEntity* entity) const;

		struct ComponentState
		{
			ComponentState()
				: name(""), entityId(G2::Entity::UNINITIALIZED_ENTITY_ID), types(G2Core::Flags::NO_FLAGS) {}

			std::string		name;				// the name
			unsigned int	entityId;			// The entity id
			G2::Studio::ComponentMask types;	// The existing core components attached to the entity id
		};

		QList<ComponentState> mComponents;
};

