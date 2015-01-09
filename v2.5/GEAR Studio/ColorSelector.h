#pragma once
#include "ui_ColorSelector.h"

#include <G2Core/Event.h>

#include <QtWidgets/QWidget>
#include <json/json.h>
#include <map>
#include <glm/glm.hpp>

/** This class implements a color selector module.
* @created	2015/01/07
* @author Andy Reimann <a.reimann@moorlands-grove.de>
*/
class ColorSelector : public QWidget
{
	Q_OBJECT
	public:
		/** Creates a new ColorSelector with the given information.
		 * @param defaultColor The default color to initialize with in case no color is available in the given source.
		 * @param source The source data to initialize the TextureSelector with.
		 */
		ColorSelector(glm::vec4 const& defaultColor, Json::Value const& source, QWidget *parent = 0);
		/** Returns the data of this ColorSelector in it's most recent state.
		* @return The data of this ColorSelector in it's most recent state.
		*/
		Json::Value const& getData() const { return mTarget; }
		/** Updates the data of this ColorSelector.
		* @param source The new data to use for initialization.
		*/
		void setData(Json::Value const& source);

		G2::Event<ColorSelector*>	onColorSelected;
		Ui::ColorSelector	ui;					// The Qt UI class for the project creation dialog

	private slots:
		void selectColor();

	private:

		void _initializeUiFromData();

		Json::Value			mTarget;			// The json object to write the information to.
		QColor				mDefaultColor;		// The default color of this ColorSelector
};
