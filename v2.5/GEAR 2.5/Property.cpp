// GEAR 2.5 - Game Engine Andy Reimann - Author: Andy Reimann <andy@moorlands-grove.de>
// (c) 2014 GEAR 2.5
#include "Property.h"

using namespace G2;


Property::Property(std::string const& name, 
				   std::string const& niceName, 
				   std::string const& dataType,
				   std::string const& defaultValue) 
	: mName(name),
	mNiceName(niceName),
	mDataType(dataType)
{
	// produce shader code
	mShaderCode.append("uniform ");
	mShaderCode.append(mDataType);
	mShaderCode.append(" ");
	mShaderCode.append(mName);
	if(mDefaultValue.length() > 0) 
	{	
		mShaderCode.append(" = ");
		mShaderCode.append(mDefaultValue);
	}
	mShaderCode.append(";");
}
