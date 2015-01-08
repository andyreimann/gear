#include "TextureSelector.h"

#include <QtWidgets/QFileDialog>
#include <QtWidgets/QColorDialog>
#include <boost/algorithm/string/predicate.hpp>
#include <boost/algorithm/string.hpp>
#include <algorithm>

TextureSelector::TextureSelector(std::string const& imagePath, std::string const& projectDirectory, QWidget *parent /*= 0*/)
	: QWidget(parent),
	mProjectDirectory(projectDirectory),
	mSelectedTexture(imagePath)
{
	ui.setupUi(this);

	connect(ui.texSelect, SIGNAL(clicked()), this, SLOT(selectTex()));

	if (mSelectedTexture.size() > 0)
	{
		QImage image(mSelectedTexture.c_str());
		if (!image.isNull())
		{
			//std::cout << ui.label_6->width() << " --- " << ui.label_6->height() << std::endl;
			mPreviewImage = image.scaled(ui.texSurf->width(), ui.texSurf->width(), Qt::KeepAspectRatio);
			ui.texSelect->setText(mSelectedTexture.c_str());
			ui.texSurf->setPixmap(QPixmap::fromImage(mPreviewImage));
		}
	}
}

TextureSelector::TextureSelector(std::string const& imagePath, std::string const& projectDirectory, QImage const& image, std::string const& samplerStr, QWidget *parent /*= 0*/)
	: QWidget(parent),
	mProjectDirectory(projectDirectory),
	mSelectedTexture(imagePath),
	mPreviewImage(image)
{
	ui.setupUi(this);

	connect(ui.texSelect, SIGNAL(clicked()), this, SLOT(selectTex()));
	if (!image.isNull())
	{
		ui.texSelect->setText(mSelectedTexture.c_str());
		ui.texSurf->setPixmap(QPixmap::fromImage(mPreviewImage));
	}
	else if (mSelectedTexture.size() > 0)
	{
		mPreviewImage = QImage(mSelectedTexture.c_str());
		if (!image.isNull())
		{
			//std::cout << ui.label_6->width() << " --- " << ui.label_6->height() << std::endl;
			mPreviewImage = mPreviewImage.scaled(ui.texSurf->width(), ui.texSurf->width(), Qt::KeepAspectRatio);
			ui.texSelect->setText(mSelectedTexture.c_str());
			ui.texSurf->setPixmap(QPixmap::fromImage(mPreviewImage));
		}
	}

	for (int i = 0; i < ui.samplerBox->count(); ++i)
	{
		std::string currentSamplerStr = ui.samplerBox->itemText(i).toStdString();
		std::transform(currentSamplerStr.begin(), currentSamplerStr.end(), currentSamplerStr.begin(), ::toupper);
		if (currentSamplerStr == samplerStr)
		{
			ui.samplerBox->setCurrentIndex(i);
			break;
		}
	}
}

void TextureSelector::selectTex()
{
	std::string dialogDir = mProjectDirectory + "/assets/textures";
	QString filePath = QFileDialog::getOpenFileName(this, "Select diffuse texture file", dialogDir.c_str());
	if (!filePath.isNull())
	{
		// 
		std::string fullPath = filePath.toStdString();

		if (!boost::algorithm::starts_with(fullPath, mProjectDirectory))
		{
			std::cout << "[Texture] Selected file is not contained in the project directory!" << std::endl;
		}
		else
		{

			// update ui
			QImage image(fullPath.c_str());
			if (!image.isNull())
			{
				//std::cout << ui.label_6->width() << " --- " << ui.label_6->height() << std::endl;
				mPreviewImage = image.scaled(ui.texSurf->width(), ui.texSurf->width(), Qt::KeepAspectRatio);
				mSelectedTexture = fullPath;
				ui.texSurf->setPixmap(QPixmap::fromImage(mPreviewImage));
				onTextureSelected(this);
			}
		}
	}
	// invoke
}

std::string
TextureSelector::getSampler() const
{
	std::string sampler = ui.samplerBox->currentText().toStdString();
	std::transform(sampler.begin(), sampler.end(), sampler.begin(), ::toupper);
	return sampler;
}

QImage const&
TextureSelector::getPreviewImage() const
{
	return mPreviewImage;
}