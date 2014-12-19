#include "PropertiesTab.h"


PropertiesTab::PropertiesTab()
	: mOpen(true),
	mTabToggle(nullptr)
{
}

void
PropertiesTab::toggleTab()
{
	if (mOpen)
	{
		mTabToggle->hide();
	}
	else
	{
		mTabToggle->show();
	}
	mOpen = !mOpen;
}