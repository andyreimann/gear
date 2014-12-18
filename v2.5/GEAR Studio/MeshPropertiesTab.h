#pragma once
#include "ui_MeshPropertiesTab.h"
#include "ManagedEntity.h"
#include <QtWidgets/QWidget>


/** This is the main class of the GEAR Editor Studio.
* @created	2014/12/17
* @author Andy Reimann <a.reimann@moorlands-grove.de>
*/
class MeshPropertiesTab : public QWidget
{
	Q_OBJECT
	public:
		MeshPropertiesTab(QWidget *parent = 0);
		~MeshPropertiesTab();



	private slots:

		void toggleTab();
		void selectMesh();

	private:

		void _init(ManagedEntity* entity);

		bool mOpen;
		Ui::MeshPropertiesTab ui;					// The Qt UI class for the project creation dialog
};
