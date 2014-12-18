#include "JsonSerializer.h"
#include <fstream>

JsonSerializer::JsonSerializer(std::string const& file) :
	mFile(file)
{
}

bool
JsonSerializer::serialize(Json::Value const& resource)
{
	std::ofstream out(mFile);
	out << resource;
	out.close();
	return true;
}