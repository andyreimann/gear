#include "TextureSelector.h"

#include <QtWidgets/QFileDialog>
#include <QtWidgets/QColorDialog>
#include <boost/algorithm/string/predicate.hpp>
#include <boost/algorithm/string.hpp>
#include <algorithm>

TextureSelector::TextureSelector(std::string const& projectDirectory, QWidget *parent /*= 0*/)
	: QWidget(parent),
	mProjectDirectory(projectDirectory)
{
	ui.setupUi(this);

	connect(ui.texSelect, SIGNAL(clicked()), this, SLOT(selectTex()));
}

TextureSelector::TextureSelector(std::string const& projectDirectory, QImage const& image, QWidget *parent /*= 0*/)
	: QWidget(parent),
	mProjectDirectory(projectDirectory)
{
	ui.setupUi(this);

	connect(ui.texSelect, SIGNAL(clicked()), this, SLOT(selectTex()));
	ui.texSurf->setPixmap(QPixmap::fromImage(image));
}

void TextureSelector::selectTex()
{
	// invoke
	onTextureSelected(this);
}