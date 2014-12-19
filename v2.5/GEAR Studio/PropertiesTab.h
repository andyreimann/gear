#pragma once
#include <QtWidgets/QPushButton>


/** This is the main class of the GEAR Editor Studio.
* @created	2014/12/17
* @author Andy Reimann <a.reimann@moorlands-grove.de>
*/
class PropertiesTab
{
	public:
		PropertiesTab();

		void toggleTab();

	private:
		bool mOpen;
	protected:
		QPushButton* mTabToggle;

};
