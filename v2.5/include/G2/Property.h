// GEAR 2.5 - Game Engine Andy Reimann - Author: Andy Reimann <andy@moorlands-grove.de>
// (c) 2014 GEAR 2.5
#pragma once
#include <string>

namespace G2 
{
	/** This class holds all informations needed to describe a uniform variable 
	 * used in a Shader and parsed from an UberShader.
	 * @created:	2014/02/09
	 * @author Andy Reimann <a.reimann@moorlands-grove.de>
	 */
	class Property
	{
		public:
			Property() {}
			Property(std::string const& name) : mName(name) {}
			/** This constructs a new Property.
			 * @param name The name of the property variable
			 * @param niceName The human readable name of the property variable (not used in final shader code)
			 * @param dataType The data type of the property.
			 * @param defaultValue The default value of the property or an empty string with a length of 0 if no default value should be used.
			 */
			Property(std::string const& name, std::string const& niceName, std::string const& dataType, std::string const& defaultValue);
			/** This function will return the name of the Property. 
			* @return The name of the Property.
			*/
			std::string const& getName() const { return mName; }
			/** This function will return the shader code produced by this LocationBinding object.
			 * @return The shader code produced by this LocationBinding object.
			 */
			std::string const& getShaderCode() const { return mShaderCode; };
			/** This function will return the DataType. 
			* @return The current DataType.
			*/
			std::string const& getDataType() const { return mDataType; }
			/** This function will return the DefaultValue. 
			* @return The current DefaultValue.
			*/
			std::string const& getDefaultValue() const { return mDefaultValue; }
		private:
			std::string		mName;			// The name of the location binding variable
			std::string		mNiceName;		// The human readable name of the location binding variable (not used in final shader code)
			std::string		mDataType;		// The datatype of the location binding
			std::string		mDefaultValue;	// The default value of the property or an empty string if no default value should be used.
			std::string		mShaderCode;	// The shader code produced by this LocationBinding object
	};
};