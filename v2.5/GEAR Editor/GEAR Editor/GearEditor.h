#ifndef GEAREDITOR_H
#define GEAREDITOR_H

#include <QtWidgets/QMainWindow>
#include "ui_geareditor.h"

class GEAREditor : public QMainWindow
{
	Q_OBJECT

public:
	GEAREditor(QWidget *parent = 0);
	~GEAREditor();

private:
	Ui::GEAREditorClass ui;
};

#endif // GEAREDITOR_H
