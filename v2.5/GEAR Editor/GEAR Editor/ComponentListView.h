#pragma once
#include <QListView>

class ComponentListView : public QListView
{
	Q_OBJECT

	public:
		ComponentListView(QWidget *parent = 0);
		~ComponentListView(void);
};

