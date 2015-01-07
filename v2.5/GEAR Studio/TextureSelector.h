#pragma once
#include "ui_TextureSelector.h"

#include <G2Core/Event.h>

#include <QtWidgets/QWidget>

/** This class implements the ManagedEntity property regarding materials.
* @created	2015/01/06
* @author Andy Reimann <a.reimann@moorlands-grove.de>
*/
class TextureSelector : public QWidget
{
	Q_OBJECT
	public:
		TextureSelector(std::string const& projectDirectory, QWidget *parent = 0);
		TextureSelector(std::string const& projectDirectory, QImage const& image, QWidget *parent = 0);

		G2::Event<TextureSelector*>	onTextureSelected;

		Ui::TextureSelector	ui;					// The Qt UI class for the project creation dialog

	private slots:
		void selectTex();

	private:
		std::string			mProjectDirectory;	// The projects directory
};
