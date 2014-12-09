#pragma once
#include "Defines.h"

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
*/
class ComponentListItemModel : public QAbstractListModel
{
	Q_OBJECT

	public:
		ComponentListItemModel(QObject *parent = 0);
		~ComponentListItemModel();
		/** Connects the ComponentListModel to the GEAR Entity-Component-System
		 * meaning that it will start receiving callbacks whenever Components are added/removed.
		 */
		void startListening();


		virtual int rowCount(QModelIndex const& parent = QModelIndex()) const override;
		virtual QVariant data(QModelIndex const& index, int role = Qt::DisplayRole) const override;
		virtual bool insertRows(int row, int count, QModelIndex const& parent = QModelIndex()) override;
		virtual bool removeRows(int row, int count, QModelIndex const& parent = QModelIndex()) override;

		bool setData(const QModelIndex &index, QVariant const& value, unsigned int entityId, G2::Studio::ComponentFlag::Name type, int role = Qt::EditRole);
	private:
		// GEAR CALLBACKS FOR CORE COMPONENTS START
		void _onRenderComponentAdded(G2::RenderSystem* system, unsigned int entityId);
		void _onSplineAnimationAdded(G2::SplineAnimationSystem* system, unsigned int entityId);
		void _onCameraComponentAdded(G2::CameraSystem* system, unsigned int entityId);
		void _onTransformComponentAdded(G2::TransformSystem* system, unsigned int entityId);

		void _onRenderComponentRemoved(G2::RenderSystem* system, unsigned int entityId);
		void _onSplineAnimationRemoved(G2::SplineAnimationSystem* system, unsigned int entityId);
		void _onCameraComponentRemoved(G2::CameraSystem* system, unsigned int entityId);
		void _onTransformComponentRemoved(G2::TransformSystem* system, unsigned int entityId);
		// GEAR CALLBACKS FOR CORE COMPONENTS END

		void _onCoreComponentAdded(unsigned int entityId, G2::Studio::ComponentFlag::Name type);
		void _onCoreComponentRemoved(unsigned int entityId, G2::Studio::ComponentFlag::Name type);

		std::string _buildDisplayName(unsigned int entityId, G2::Studio::ComponentMask types) const;

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

