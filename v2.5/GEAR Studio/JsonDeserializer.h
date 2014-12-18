#pragma once
#include <json/json.h>


/** This interface can be used to obtain JSON-Serialization features.
* @created	2014/12/17
* @author Andy Reimann <a.reimann@moorlands-grove.de>
*/
class JsonDeserializer
{
	public:
		JsonDeserializer(std::string const& file);
		/** Returns if the last loading process was finished successfully.
		 * @return True if the process encountered an error during loading, false if not.
		 */
		bool error() const { return mParseError;  }

		/** Returns the last captured error string.
		* @return The last captured error string.
		*/
		std::string const& getLastErrorMessage() const { return mParseErrorMsg;  }


		virtual ~JsonDeserializer() {}

	private:

		void _parse();
		
		std::string mFile;
		std::string	mParseErrorMsg;
		bool		mParseError;

	protected:
		Json::Value mResource;

};
