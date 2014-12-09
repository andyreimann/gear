#pragma once
#include <QtWidgets/QListView>

/** This class acts as a view for the component list of the editor.
 * The component list is a list of entries, where every entry represents a group of components
 * attached to a single G2::Entity.
 */
class ComponentListView : public QListView
{
	Q_OBJECT

	public:
		ComponentListView(QWidget *parent = 0);
		~ComponentListView(void);
};

