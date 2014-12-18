#pragma once
#include <json/json.h>


/** This interface can be used to obtain JSON-Deserialization features.
* @created	2014/12/17
* @author Andy Reimann <a.reimann@moorlands-grove.de>
*/
class JsonSerializer
{
	public:
		JsonSerializer(std::string const& file);

		bool serialize(Json::Value const& resource);

		virtual ~JsonSerializer() {}
	private:
		
		std::string mFile;

};
