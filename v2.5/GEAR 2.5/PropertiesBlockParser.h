// GEAR 2.5 - Game Engine Andy Reimann - Author: Andy Reimann <andy@moorlands-grove.de>
// (c) 2014 GEAR 2.5
#pragma once
#include "Property.h"

#include <string>
#include <vector>

namespace G2 
{
	class FileResource;
	struct ShaderMetaData;
	/** This class takes care of loading the content of an Properties block
	 * inside of an UberShader file.
	 * @created:	2014/02/09
	 * @author Andy Reimann <a.reimann@moorlands-grove.de>
	 */
	class PropertiesBlockParser 
	{
		public:
			/** This constructs a new PropertiesBlockParser.
			 */
			PropertiesBlockParser(FileResource* file);
			void parse(ShaderMetaData* shaderMetaData);
			/** This function will return the Properties. 
			* @return The current Properties.
			*/
			std::vector<Property> const& getProperties() const { return mProperties; }
		protected:
		private:
			/** This function will check the given values for a Property,
			 * create some ShaderMetaData if needed and add a new Property 
			 * value to the UberShader.
			 * @param name The name of the Property.
			 * @param niceName The human readable name of the Property.
			 * @param dataType The data type of the Property.
			 * @param defaultValue The read default value for the Property.
			 * @param shaderMetaData The meta data to write to.
			 */
			void checkAndCreateMetaData(std::string const& name, 
										std::string const& niceName, 
										std::string const& dataType, 
										std::string const& defaultValue,
										ShaderMetaData* shaderMetaData);

			FileResource*			mFile;			// The FileResource to parse from.
			std::vector<Property>	mProperties;	// The parsed properties
	};
};