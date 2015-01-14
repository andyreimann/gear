#pragma once
#include "ui_ManagedEntitiesTab.h"

#include <QtWidgets/QWidget>

class Scene;
class ManagedEntity;
/** This class implements the loaded ManagedEntity tab functionality of the editor.
* @created	2015/01/14
* @author Andy Reimann <a.reimann@moorlands-grove.de>
*/
class ManagedEntitiesTab : public QWidget
{
	Q_OBJECT
	public:
		ManagedEntitiesTab(QWidget *parent = 0);
		~ManagedEntitiesTab();

	private slots:
		void itemSelected(QItemSelection const& selected, QItemSelection const& deselected);
	private:
		/** This callback function will be called whenever a scene is unloaded.
		 * It will trigger a deletion of all existing rows in the model.
		 */
		void _onSceneUnloaded(Scene* scene);
		/** This callback function will be called whenever a scene is loaded.
		 */
		void _onSceneLoaded(Scene* scene);
		/** This callback function will be called whenever a ManagedEntity is newly added to the Scene.
		 * It will trigger an insertion of a new row in the model.
		 */
		void _onManagedEntityCreated(Scene* scene, ManagedEntity* entity);
		/** This callback function will be called whenever a ManagedEntity is selected.
		 */
		void _onManagedEntitySelected(ManagedEntity* entity);
		/** This callback function will be called whenever a ManagedEntity is newly added to the Scene.
		 * It will trigger a remove operation of an existing row in the model.
		 */
		void _onManagedEntityRemoved(Scene* scene, ManagedEntity* entity);

		
		Ui::ManagedEntitiesTab	ui;
		Scene*					mScene;
};
