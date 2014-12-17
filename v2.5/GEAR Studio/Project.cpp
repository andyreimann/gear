#include "Project.h"

Project::Project(std::string const& projectFile) :
	mProjectFile(projectFile),
	mParseError(false)
{
	_parse();
}

void
Project::_parse()
{
	mParseError = false; // reset
	mParseErrorMsg = "";
	Json::Reader reader;
	bool parsingSuccessful = reader.parse(mProjectFile, mProjectResource);
	if (!parsingSuccessful)
	{
		// report to the user the failure and their locations in the document.
		mParseErrorMsg = reader.getFormattedErrorMessages();
		mParseError = true;
	}
}