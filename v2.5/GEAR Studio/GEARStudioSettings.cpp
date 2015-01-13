#include "GEARStudioSettings.h"
#include "GEARStudioEvents.h"


GEARStudioSettings::GEARStudioSettings(std::string const& settingsFile) :
	JsonDeserializer(settingsFile),
	JsonSerializer(settingsFile)
{
	if (this->error())
	{
		G2S::onLog(WARNING, "Could not load GEAR Studio settings from file " + settingsFile + ": " + this->getLastErrorMessage());
	}
}

void
GEARStudioSettings::save()
{
	if (!error())
	{
		serialize(mResource);
	}
}