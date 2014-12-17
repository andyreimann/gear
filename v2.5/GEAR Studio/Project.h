#pragma once
#include <json/json.h>


/** This class holds all information about a project.
* @created	2014/12/17
* @author Andy Reimann <a.reimann@moorlands-grove.de>
*/
class Project
{
	public:
		/** This will create a new Project instance from a given project file.
		 * @param projectFile The project file to load.
		 */
		Project(std::string const& projectFile);
		/** Returns if the last loading process of the Project was finished sucessfully.
		 * @return True if the Project encountered an error during loading, false if not.
		 */
		bool error() const { return mParseError;  }
 
		std::string const& getLastErrorMessage() const { return mParseErrorMsg;  }



	private:

		void _parse();
		
		std::string mProjectFile;
		Json::Value mProjectResource;
		std::string	mParseErrorMsg;
		bool		mParseError;

};
