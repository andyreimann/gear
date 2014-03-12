// GEAR 2.5 - Game Engine Andy Reimann - Author: Andy Reimann <andy@moorlands-grove.de>
// (c) 2014 GEAR 2.5
#include "LocationBinding.h"

using namespace G2;

LocationBinding::LocationBinding(Semantics::Name semantic, 
								 std::string const& name, 
								 std::string const& niceName, 
								 std::string const& dataType)
	: mSemantic(semantic),
	mName(name),
	mNiceName(niceName),
	mDataType(dataType)
{
	// produce shader code
	mShaderCode.append("layout(location=");
	mShaderCode.append(std::to_string((long long)static_cast<int>(mSemantic))); // convert semantic to int
	mShaderCode.append(") in ");
	mShaderCode.append(mDataType);
	mShaderCode.append(" ");
	mShaderCode.append(mName);
	mShaderCode.append(";");
}
