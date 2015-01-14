#pragma once
#include "ui_ManagedEntityPanel.h"

#include <QtWidgets/QWidget>

class Scene;
/** This class provides the functionality for the ManagedEntity list.
* @created	2015/01/15
* @author Andy Reimann <a.reimann@moorlands-grove.de>
*/
class ManagedEntityPanel : public QWidget
{
	Q_OBJECT
	public:
		/** Creates a new ManagedEntityPanel with the given information.
		 * @param currentScene The currently loaded Scene.
		 * @param entityId The entity ID of the ManagedEntity, this instance is responsible for.
		 */
		ManagedEntityPanel(Scene* currentScene, unsigned int entityId, QWidget *parent = 0);
		
	private slots:

	private:

		Ui::ManagedEntityPanel	ui;					// The Qt UI class for the project creation dialog
		Scene*				mScene;				// The currently loaded Scene
		unsigned int		mEntityId;			// The entity ID of the ManagedEntity, this instance is responsible for.
};
