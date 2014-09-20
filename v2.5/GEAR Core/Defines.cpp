#include "Defines.h"

using namespace G2Core;

Semantics::Name Semantics::parse(std::string const& name)
{
	if(name == "POSITION") 
		return POSITION;
	else if(name == "BLENDWEIGHT") 
		return BLENDWEIGHT;
	else if(name == "NORMAL") 
		return NORMAL;
	else if(name == "COLOR_0") 
		return COLOR_0;
	else if(name == "COLOR_1") 
		return COLOR_1;
	else if(name == "FOGCOORD") 
		return FOGCOORD;
	else if(name == "PSIZE") 
		return PSIZE;
	else if(name == "BLENDINDICES") 
		return BLENDINDICES;
	else if(name == "TEXCOORD_0") 
		return TEXCOORD_0;
	else if(name == "TEXCOORD_1") 
		return TEXCOORD_1;
	else if(name == "TEXCOORD_2") 
		return TEXCOORD_2;
	else if(name == "TEXCOORD_3") 
		return TEXCOORD_3;
	else if(name == "TEXCOORD_4") 
		return TEXCOORD_4;
	else if(name == "TEXCOORD_5") 
		return TEXCOORD_5;
	else if(name == "TANGENT") 
		return TANGENT;
	else if(name == "BINORMAL") 
		return BINORMAL;
	return SEMANTIC_INVALID;
}