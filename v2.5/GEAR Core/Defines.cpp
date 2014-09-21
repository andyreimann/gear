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

FrameBufferAttachmentPoint::Name
FrameBufferAttachmentPoint::getByDataFormat(std::string const& outputFormat) 
{
	if(outputFormat == "RGB") 
	{
		return COLOR_0;
	}
	else if(outputFormat == "RGBA") 
	{
		return COLOR_0;
	}
	else if(outputFormat == "DEPTH") 
	{
		return DEPTH;
	}
	else if(outputFormat == "STENCIL") 
	{
		return STENCIL;
	}

	if(outputFormat == "RGB") { return COLOR_0; }
	if(outputFormat == "RGB_UB") { return COLOR_0; }
	if(outputFormat == "RGB_US") { return COLOR_0; }
	if(outputFormat == "RGBA") { return COLOR_0; }
	if(outputFormat == "RGBA_UB") { return COLOR_0; }
	if(outputFormat == "RGBA_US") { return COLOR_0; }
	if(outputFormat == "RGBA16_F") { return COLOR_0; }
	if(outputFormat == "RGBA_F") { return COLOR_0; }
	if(outputFormat == "RGBA_B") { return COLOR_0; }
	if(outputFormat == "RGBA_S") { return COLOR_0; }
	if(outputFormat == "RGBA_I") { return COLOR_0; }
	if(outputFormat == "RGBA_UI") { return COLOR_0; }
	if(outputFormat == "ALPHA_UB") { return COLOR_0; }
	if(outputFormat == "ALPHA_US") { return COLOR_0; }
	if(outputFormat == "ALPHA16_F") { return COLOR_0; }
	if(outputFormat == "ALPHA_B") { return COLOR_0; }
	if(outputFormat == "ALPHA_S") { return COLOR_0; }
	if(outputFormat == "ALPHA_I") { return COLOR_0; }
	if(outputFormat == "ALPHA_UI") { return COLOR_0; }
	if(outputFormat == "DEPTH") { return DEPTH; }
	if(outputFormat == "DEPTH16") { return DEPTH; }
	if(outputFormat == "DEPTH24") { return DEPTH; }
	if(outputFormat == "DEPTH32") { return DEPTH; }
	if(outputFormat == "LUMINANCE") { return COLOR_0; }
	if(outputFormat == "LUMINANCE_ALPHA_UB") { return COLOR_0; }
	if(outputFormat == "LUMINANCE_ALPHA_US") { return COLOR_0; }
	if(outputFormat == "LUMINANCE_ALPHA16_F") { return COLOR_0; }
	if(outputFormat == "LUMINANCE_ALPHA_F") { return COLOR_0; }
	if(outputFormat == "LUMINANCE_ALPHA_B") { return COLOR_0; }
	if(outputFormat == "LUMINANCE_ALPHA_S") { return COLOR_0; }
	if(outputFormat == "LUMINANCE_ALPHA_I") { return COLOR_0; }
	if(outputFormat == "LUMINANCE_ALPHA_UI") { return COLOR_0; }
	if(outputFormat == "LUMINANCE_UB") { return COLOR_0; }
	if(outputFormat == "LUMINANCE_US") { return COLOR_0; }
	if(outputFormat == "LUMINANCE16_F") { return COLOR_0; }
	if(outputFormat == "LUMINANCE_F") { return COLOR_0; }
	if(outputFormat == "LUMINANCE_B") { return COLOR_0; }
	if(outputFormat == "LUMINANCE_S") { return COLOR_0; }
	if(outputFormat == "LUMINANCE_I") { return COLOR_0; }
	if(outputFormat == "LUMINANCE_UI") { return COLOR_0; }
	if(outputFormat == "INTENSITY") { return COLOR_0; }
	if(outputFormat == "INTENSITY_UB") { return COLOR_0; }
	if(outputFormat == "INTENSITY_US") { return COLOR_0; }
	if(outputFormat == "INTENSITY16_F") { return COLOR_0; }
	if(outputFormat == "INTENSITY_F") { return COLOR_0; }
	if(outputFormat == "INTENSITY_B") { return COLOR_0; }
	if(outputFormat == "INTENSITY_S") { return COLOR_0; }
	if(outputFormat == "INTENSITY_I") { return COLOR_0; }
	if(outputFormat == "INTENSITY_UI") { return COLOR_0; }

	return ATTACHMENT_INVALID;
}

WrapMode::Name
WrapMode::parse(std::string const& name) 
{
	if(name == "CLAMP_TO_EDGE") 
	{
		return CLAMP_TO_EDGE;
	}
	if(name == "CLAMP_TO_BORDER") 
	{
		return CLAMP_TO_BORDER;
	}
	else if(name == "MIRRORED_REPEAT") 
	{
		return MIRRORED_REPEAT;
	}
	else if(name == "MIRROR_CLAMP_TO_EDGE") 
	{
		return MIRROR_CLAMP_TO_EDGE;
	}
	else if(name == "REPEAT") 
	{
		return REPEAT;
	}
	return WRAP_MODE_INVALID;
}