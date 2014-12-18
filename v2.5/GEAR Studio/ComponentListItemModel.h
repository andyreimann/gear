#pragma once
#include "Defines.h"
#include "GEARStudioEvents.h"

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
		bool setData(const QModelIndex &index, QVariant const& value, ManagedEntity* entity, int role = Qt::EditRole);

		ManagedEntity* entity(QModelIndex const& index);
	private:
		/** This callback function will be called whenever a scene is unloaded.
		 * It will trigger a deletion of all existing rows in the model.
		 */
		void _onSceneUnloaded(Scene* scene);
		/** This callback function will be called whenever a ManagedEntity is newly added to the Scene.
		* It will trigger an insertion of a new row in the model.
		*/
		void _onManagedEntityCreated(Scene* scene, ManagedEntity* entity);
		/** This callback function will be called whenever a ManagedEntity is newly added to the Scene.
		* It will trigger a remove operation of an existing row in the model.
		*/
		void _onManagedEntityRemoved(Scene* scene, ManagedEntity* entity);

		QList<ManagedEntity*> mComponents;
};

