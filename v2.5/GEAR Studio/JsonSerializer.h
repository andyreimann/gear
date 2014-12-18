#pragma once
#include <json/json.h>


/** This interface can be used to obtain JSON-Deserialization features.
* @created	2014/12/17
* @author Andy Reimann <a.reimann@moorlands-grove.de>
*/
class JsonSerializer
{
	public:
		/** Creates a new JsonSerializer telling it the name to serialize to.
		 * Please note that when creating a JsonSerializer, nothing is serialized at all.
		 */
		JsonSerializer(std::string const& file);

		virtual ~JsonSerializer() {}

	protected:
		bool serialize(Json::Value const& resource);
	private:
		
		std::string mFile;

};
