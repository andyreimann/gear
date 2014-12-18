#include "JsonDeserializer.h"

#include <G2/FileResource.h>

JsonDeserializer::JsonDeserializer(std::string const& file) :
	mFile(file),
	mParseError(false)
{
	_parse();
}

void
JsonDeserializer::_parse()
{
	mParseError = false; // reset
	mParseErrorMsg = "";
	Json::Reader reader;

	G2::FileResource file(mFile);
	if (file.isOpen())
	{
		std::string content;
		while (!file.eof())
		{
			content += file.getLine();
		}
		bool parsingSuccessful = reader.parse(content, mResource);
		if (!parsingSuccessful)
		{
			// report to the user the failure and their locations in the document.
			mParseErrorMsg = reader.getFormattedErrorMessages();
			mParseError = true;
		}
	}
	else
	{
		mParseError = true;
		mParseErrorMsg = "Error: Could not open file " + mFile;
	}
}