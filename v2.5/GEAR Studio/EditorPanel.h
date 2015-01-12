#pragma once
#include "ui_EditorPanel.h"

#include <G2/GEAR.h>
#include <QtWidgets/QWidget>

class ManagedEntity;
class GEAREditor;
/** This class implements the EditorPanel with all of it's functionality.
 * @created	2015/01/12
 * @author Andy Reimann <a.reimann@moorlands-grove.de>
 */
class EditorPanel : public QWidget
{
	Q_OBJECT
	public:
		/** Creates a new EditorPanel.
		 * @param editor A pointer to the GEAREditor instance.
		 */
		EditorPanel(GEAREditor* editor, QWidget *parent = 0);
		~EditorPanel();

	private slots:
		/** Changes the polygon draw mode of the currently selected ManagedEntity 
		 * to G2Core::PolygonDrawMode::FILL.
		 */
		void renderSolid();
		/** Changes the polygon draw mode of the currently selected ManagedEntity
		 * to G2Core::PolygonDrawMode::LINE.
		 */
		void renderWire();
		/** Changes the polygon draw mode of the currently selected ManagedEntity
		 * to G2Core::PolygonDrawMode::POINT.
		 */
		void renderPoint();

	private:
		/** This function will initialize the UI of the EditorPanel according
		 * to the currently selected ManagedEntity instance.
		 */
		void _initUi();
		/** Callback for a select action of a ManagedEntity instance.
		 * @param entity A pointer to the ManagedEntity instance, which was selected.
		 */
		void _onManagedEntitySelected(ManagedEntity* entity);
		/** Changes the polygon draw mode of the currently selected ManagedEntity
		 * to the given one.
		 * @param drawMode The polygon draw mode to set.
		 */
		void _changePolygonDrawMode(G2Core::PolygonDrawMode::Name drawMode);

		GEAREditor*		mEditor;			// The instance of the editor
		Ui::EditorPanel	ui;					// The Qt UI class for the EditorPanel
		ManagedEntity*	mEntity;			// The currently selected Entity
};
