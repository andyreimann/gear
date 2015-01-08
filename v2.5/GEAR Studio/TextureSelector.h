#pragma once
#include "ui_TextureSelector.h"

#include <G2Core/Event.h>
#include <G2/Sampler.h>

#include <QtWidgets/QWidget>
#include <json/json.h>
#include <map>

/** This class implements a texture selector module.
* @created	2015/01/07
* @author Andy Reimann <a.reimann@moorlands-grove.de>
*/
class TextureSelector : public QWidget
{
	Q_OBJECT
	public:
		TextureSelector(Json::Value const& target, std::string const& projectDirectory, QWidget *parent = 0);

		Json::Value const& getData() const { return mTarget; }

		G2::Event<TextureSelector*>	onTextureSelected;
		G2::Event<TextureSelector*>	onSamplerSelected;
		G2::Event<TextureSelector*>	onRemoveSelector;
		
		Ui::TextureSelector	ui;					// The Qt UI class for the project creation dialog


	private slots:
		void selectTex();
		void samplerChanged(int idx);
		void removeSelector();

	private:

		void _initSampler(std::string const& samplerStr);
		void _initPreview(std::string const& imagePath, bool useCache);
		std::string _getSampler() const;

		std::string			mProjectDirectory;	// The projects directory
		Json::Value			mTarget;			// The json object to write the information to.
		
		static std::map<std::string, QImage>	gImageCache;	// The internal cache for the preview images

};
