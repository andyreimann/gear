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

RenderLayer::RenderLayerMask
G2Core::RenderLayer::getByRenderLayer(std::string const& renderLayer)
{
	if (renderLayer == "TERRAIN")
	{
		return TERRAIN;
	}
	else if (renderLayer == "WATER")
	{
		return WATER;
	}
	else if (renderLayer == "TRANSPARENT_MESH")
	{
		return TRANSPARENT_MESH;
	}
	else if (renderLayer == "STATIC_MESH")
	{
		return STATIC_MESH;
	}
	else if (renderLayer == "DYNAMIC_MESH")
	{
		return DYNAMIC_MESH;
	}
	else if (renderLayer == "LAYER_5")
	{
		return LAYER_5;
	}
	else if (renderLayer == "LAYER_6")
	{
		return LAYER_6;
	}
	else if (renderLayer == "LAYER_7")
	{
		return LAYER_7;
	}
	else if (renderLayer == "LAYER_8")
	{
		return LAYER_8;
	}
	else if (renderLayer == "LAYER_9")
	{
		return LAYER_9;
	}
	else if (renderLayer == "LAYER_10")
	{
		return LAYER_10;
	}
	else if (renderLayer == "LAYER_11")
	{
		return LAYER_11;
	}
	else if (renderLayer == "LAYER_12")
	{
		return LAYER_12;
	}
	else if (renderLayer == "LAYER_13")
	{
		return LAYER_13;
	}
	else if (renderLayer == "LAYER_14")
	{
		return LAYER_14;
	}
	else if (renderLayer == "LAYER_15")
	{
		return LAYER_15;
	}
	else if (renderLayer == "LAYER_16")
	{
		return LAYER_16;
	}
	else if (renderLayer == "LAYER_17")
	{
		return LAYER_17;
	}
	else if (renderLayer == "LAYER_18")
	{
		return LAYER_18;
	}
	else if (renderLayer == "LAYER_19")
	{
		return LAYER_19;
	}
	else if (renderLayer == "LAYER_20")
	{
		return LAYER_20;
	}
	else if (renderLayer == "LAYER_21")
	{
		return LAYER_21;
	}
	else if (renderLayer == "LAYER_22")
	{
		return LAYER_22;
	}
	else if (renderLayer == "LAYER_23")
	{
		return LAYER_23;
	}
	else if (renderLayer == "LAYER_24")
	{
		return LAYER_24;
	}
	else if (renderLayer == "LAYER_25")
	{
		return LAYER_25;
	}
	else if (renderLayer == "LAYER_26")
	{
		return LAYER_26;
	}
	else if (renderLayer == "LAYER_27")
	{
		return LAYER_27;
	}
	else if (renderLayer == "LAYER_28")
	{
		return LAYER_28;
	}
	else if (renderLayer == "LAYER_29")
	{
		return LAYER_29;
	}
	else if (renderLayer == "LAYER_30")
	{
		return LAYER_30;
	}
	return 0;
}