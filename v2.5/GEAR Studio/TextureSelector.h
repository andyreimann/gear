#pragma once
#include "ui_TextureSelector.h"

#include <G2Core/Event.h>
#include <G2/Sampler.h>

#include <QtWidgets/QWidget>

/** This class implements the ManagedEntity property regarding materials.
* @created	2015/01/06
* @author Andy Reimann <a.reimann@moorlands-grove.de>
*/
class TextureSelector : public QWidget
{
	Q_OBJECT
	public:
		TextureSelector(std::string const& imagePath, std::string const& projectDirectory, QWidget *parent = 0);
		TextureSelector(std::string const& imagePath, std::string const& projectDirectory, QImage const& image, std::string const& samplerStr, QWidget *parent = 0);

		G2::Event<TextureSelector*>	onTextureSelected;

		std::string const& getSelectedTexturePath() const { return mSelectedTexture; }

		QImage const& getPreviewImage() const;
		std::string getSampler() const;
		Ui::TextureSelector	ui;					// The Qt UI class for the project creation dialog

	private slots:
		void selectTex();

	private:
		std::string			mProjectDirectory;	// The projects directory

		std::string			mSelectedTexture;	// the full path of the selected texture
		QImage				mPreviewImage;		// The preview image of the selected texture


};
