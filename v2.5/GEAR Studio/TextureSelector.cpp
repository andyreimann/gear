#include "TextureSelector.h"

#include <QtWidgets/QFileDialog>
#include <QtWidgets/QColorDialog>
#include <boost/algorithm/string/predicate.hpp>
#include <boost/algorithm/string.hpp>
#include <algorithm>

static const std::string TEXTURES_PATH = "path";
static const std::string TEXTURES_SAMPLER = "sampler";

std::map<std::string, QImage> TextureSelector::gImageCache;

TextureSelector::TextureSelector(Json::Value const& target, std::string const& projectDirectory, QWidget *parent)
	: QWidget(parent),
	mProjectDirectory(projectDirectory),
	mTarget(target)
{
	ui.setupUi(this);

	connect(ui.texSelect, SIGNAL(clicked()), this, SLOT(selectTex()));
	connect(ui.samplerBox, SIGNAL(currentIndexChanged(int)), this, SLOT(samplerChanged(int)));
	connect(ui.deleteTextureSelector, SIGNAL(clicked()), this, SLOT(removeSelector()));

	ui.texSurf->setPixmap(QPixmap::fromImage(QImage()));

	if (mTarget.isMember(TEXTURES_PATH))
	{
		_initSampler(mTarget[TEXTURES_SAMPLER].asString());

		std::string fullPath = mProjectDirectory + mTarget[TEXTURES_PATH].asString();

		_initPreview(fullPath, true);
	}
}

void TextureSelector::_initSampler(std::string const& samplerStr)
{
	ui.samplerBox->setCurrentIndex(0);
	for (int i = 0; i < ui.samplerBox->count(); ++i)
	{
		std::string itemText = ui.samplerBox->itemText(i).toStdString();
		std::transform(itemText.begin(), itemText.end(), itemText.begin(), ::toupper);
		if (itemText == samplerStr)
		{
			ui.samplerBox->setCurrentIndex(i);
			return;
		}
	}
}

void TextureSelector::_initPreview(std::string const& imagePath, bool useCache)
{
	QImage image;
	if (useCache && gImageCache.count(imagePath) == 1)
	{
		image = gImageCache[imagePath];
	}
	else
	{
		image = QImage(imagePath.c_str());
		if (!image.isNull())
		{
			//std::cout << ui.label_6->width() << " --- " << ui.label_6->height() << std::endl;
			image = image.scaled(ui.texSurf->width(), ui.texSurf->width(), Qt::KeepAspectRatio);
			gImageCache[imagePath] = image;
		}
	}
	ui.texSurf->setPixmap(QPixmap::fromImage(image));
}

void TextureSelector::selectTex()
{
	std::string dialogDir = mProjectDirectory + "/assets/textures";
	QString filePath = QFileDialog::getOpenFileName(this, "Select diffuse texture file", dialogDir.c_str());
	if (!filePath.isNull())
	{
		// 
		std::string path = filePath.toStdString();

		if (!boost::algorithm::starts_with(path, mProjectDirectory))
		{
			std::cout << "[Texture] Selected file is not contained in the project directory!" << std::endl;
		}
		else
		{
			// strip project directory
			std::string fullPath = path;
			boost::replace_first(path, mProjectDirectory, "");
			// save in Json
			mTarget[TEXTURES_PATH] = path;
			mTarget[TEXTURES_SAMPLER] = _getSampler();
			// update preview without using the cached version!
			_initPreview(fullPath, false);
			// invoke event to inform all listeners that a new texture was selected
			onTextureSelected(this);
		}
	}
	// invoke
}

void TextureSelector::samplerChanged(int idx)
{
	mTarget[TEXTURES_SAMPLER] = _getSampler();
	onSamplerSelected(this);
}

void TextureSelector::removeSelector()
{
	onRemoveSelector(this);
}

std::string TextureSelector::_getSampler() const
{
	std::string sampler = ui.samplerBox->currentText().toStdString();
	std::transform(sampler.begin(), sampler.end(), sampler.begin(), ::toupper);
	return sampler;
}