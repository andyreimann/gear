#include "ColorSelector.h"

#include <QtWidgets/QColorDialog>

ColorSelector::ColorSelector(glm::vec4 const& defaultColor, Json::Value const& source, QWidget *parent)
	: QWidget(parent),
	mTarget(source),
	mDefaultColor(
	(int)(defaultColor.r * 255.f),
	(int)(defaultColor.g * 255.f),
	(int)(defaultColor.b * 255.f),
	(int)(defaultColor.a * 255.f))
{
	ui.setupUi(this);
	
	connect(ui.colorSelect, SIGNAL(clicked()), this, SLOT(selectColor()));

	_initializeUiFromData();
}

void ColorSelector::selectColor()
{
	// calculate the selected alpha separately, since it is not stored in the background palette.
	int alpha = mDefaultColor.alpha();
	if (mTarget.isMember("a"))
	{
		alpha = mTarget["a"].asInt();
	}
	QColor color = ui.colorSurf->palette().color(QPalette::Background);
	color.setAlpha(alpha);
	color = QColorDialog::getColor(color, this, "Select color", QColorDialog::ShowAlphaChannel);

	if (color.isValid())
	{
		QPalette pal;
		pal.setColor(QPalette::Background, color);
		ui.colorSurf->setPalette(pal);

		mTarget["r"] = color.red();
		mTarget["g"] = color.green();
		mTarget["b"] = color.blue();
		mTarget["a"] = color.alpha();
		// invoke event
		onColorSelected(this);
	}
}

void ColorSelector::setData(Json::Value const& source)
{
	mTarget = source;
	// initialize UI
	_initializeUiFromData();
}

void ColorSelector::_initializeUiFromData()
{
	QPalette pal;
	QColor color = mDefaultColor;
	if (mTarget.isMember("r") &&
		mTarget.isMember("g") &&
		mTarget.isMember("b") &&
		mTarget.isMember("a"))
	{
		QColor color(mTarget["r"].asInt(), mTarget["r"].asInt(), mTarget["r"].asInt(), mTarget["r"].asInt());
	}
	pal.setColor(QPalette::Background, color);
	ui.colorSurf->blockSignals(true); ui.colorSurf->setPalette(pal); ui.colorSurf->blockSignals(false);
}