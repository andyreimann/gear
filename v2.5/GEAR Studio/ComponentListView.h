#pragma once
#include <QtWidgets/QListView>

/** This class acts as a view for the component list of the editor.
 * The component list is a list of entries, where every entry represents a group of components
 * attached to a single G2::Entity.
 * @created	2014/12/09
 * @author Andy Reimann <a.reimann@moorlands-grove.de>
 */
class ComponentListView : public QListView
{
	Q_OBJECT

	public:
		ComponentListView(QWidget *parent = 0);
		~ComponentListView(void);

	protected slots:
	virtual void currentChanged(const QModelIndex & current, const QModelIndex & previous);
		

};

