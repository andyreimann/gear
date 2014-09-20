// GEAR 2.5 - Game Engine Andy Reimann - Author: Andy Reimann <andy@moorlands-grove.de>
// (c) 2014 GEAR 2.5
#include "LocationBinding.h"

using namespace G2;

LocationBinding::LocationBinding(G2Core::Semantics::Name semantic, 
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

G2Core::SemanticDataTypes::Name
G2::LocationBinding::getSemanticDataTypeByLocationBindingDataType(std::string const& locationBindingDataType) 
{
	if(locationBindingDataType == "vec4")
		return G2Core::SemanticDataTypes::RGBA_FLOAT;
	else if(locationBindingDataType == "ivec4")
		return G2Core::SemanticDataTypes::RGBA_INT;
	else if(locationBindingDataType == "vec3")
		return G2Core::SemanticDataTypes::RGB_FLOAT;
	else if(locationBindingDataType == "ivec3")
		return G2Core::SemanticDataTypes::RGB_INT;
	else if(locationBindingDataType == "vec2")
		return G2Core::SemanticDataTypes::RG_FLOAT;
	else if(locationBindingDataType == "ivec2")
		return G2Core::SemanticDataTypes::RG_INT;
	else if(locationBindingDataType == "float")
		return G2Core::SemanticDataTypes::R_FLOAT;
	else if(locationBindingDataType == "int")
		return G2Core::SemanticDataTypes::R_INT;
	return G2Core::SemanticDataTypes::SEMANTIC_DATATYPE_INVALID;
}

G2Core::VertexInputElement
LocationBinding::getVertexInputElement() const 
{
	G2Core::VertexInputElement vie;
	vie.semantic = mSemantic;
	vie.format = getSemanticDataTypeByLocationBindingDataType(mDataType);
	return vie;
}