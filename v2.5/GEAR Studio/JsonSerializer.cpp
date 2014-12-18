#include "JsonSerializer.h"

JsonSerializer::JsonSerializer(std::string const& file) :
	mFile(file)
{
}

bool
JsonSerializer::serialize(Json::Value const& resource)
{
	return false;
}