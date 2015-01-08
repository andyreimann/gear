#include "Defines.h"

using namespace G2Core;

std::string Buffer::toString(Buffer::Name name)
{
	if (name == DEPTH)
		return "DEPTH";
	if (name == STENCIL)
		return "STENCIL";
	if (name == COLOR)
		return "COLOR";
	return "";
}

Buffer::Name Buffer::fromString(std::string const& name)
{
	if (name == "DEPTH")
		return DEPTH;
	if (name == "STENCIL")
		return STENCIL;
	if (name == "COLOR")
		return COLOR;
	return BUFFER_INVALID;
}

std::string Semantics::toString(Semantics::Name name)
{
	if (name == POSITION)
		return "POSITION";
	if (name == BLENDWEIGHT)
		return "BLENDWEIGHT";
	if (name == NORMAL)
		return "NORMAL";
	if (name == COLOR_0)
		return "COLOR_0";
	if (name == COLOR_1)
		return "COLOR_1";
	if (name == FOGCOORD)
		return "FOGCOORD";
	if (name == PSIZE)
		return "PSIZE";
	if (name == BLENDINDICES)
		return "BLENDINDICES";
	if (name == TEXCOORD_0)
		return "TEXCOORD_0";
	if (name == TEXCOORD_1)
		return "TEXCOORD_1";
	if (name == TEXCOORD_2)
		return "TEXCOORD_2";
	if (name == TEXCOORD_3)
		return "TEXCOORD_3";
	if (name == TEXCOORD_4)
		return "TEXCOORD_4";
	if (name == TEXCOORD_5)
		return "TEXCOORD_5";
	if (name == BINORMAL)
		return "BINORMAL";
	if (name == TANGENT)
		return "TANGENT";
	return "";
}

Semantics::Name Semantics::fromString(std::string const& name)
{
	if (name == "POSITION")
		return POSITION;
	if (name == "BLENDWEIGHT")
		return BLENDWEIGHT;
	if (name == "NORMAL")
		return NORMAL;
	if (name == "COLOR_0")
		return COLOR_0;
	if (name == "COLOR_1")
		return COLOR_1;
	if (name == "FOGCOORD")
		return FOGCOORD;
	if (name == "PSIZE")
		return PSIZE;
	if (name == "BLENDINDICES")
		return BLENDINDICES;
	if (name == "TEXCOORD_0")
		return TEXCOORD_0;
	if (name == "TEXCOORD_1")
		return TEXCOORD_1;
	if (name == "TEXCOORD_2")
		return TEXCOORD_2;
	if (name == "TEXCOORD_3")
		return TEXCOORD_3;
	if (name == "TEXCOORD_4")
		return TEXCOORD_4;
	if (name == "TEXCOORD_5")
		return TEXCOORD_5;
	if (name == "TANGENT")
		return TANGENT;
	if (name == "BINORMAL")
		return BINORMAL;
	return SEMANTIC_INVALID;
}

std::string SemanticDataTypes::toString(SemanticDataTypes::Name name)
{
	if (name == RGBA_FLOAT)
		return "RGBA_FLOAT";
	if (name == RGBA_INT)
		return "RGBA_INT";
	if (name == RGB_FLOAT)
		return "RGB_FLOAT";
	if (name == RGB_INT)
		return "RGB_INT";
	if (name == RG_FLOAT)
		return "RG_FLOAT";
	if (name == RG_INT)
		return "RG_INT";
	if (name == R_FLOAT)
		return "R_FLOAT";
	if (name == R_INT)
		return "R_INT";
	return "";
}

SemanticDataTypes::Name SemanticDataTypes::fromString(std::string const& name)
{
	if (name == "RGBA_FLOAT")
		return RGBA_FLOAT;
	if (name == "RGBA_INT")
		return RGBA_INT;
	if (name == "RGB_FLOAT")
		return RGB_FLOAT;
	if (name == "RGB_INT")
		return RGB_INT;
	if (name == "RG_FLOAT")
		return RG_FLOAT;
	if (name == "RG_INT")
		return RG_INT;
	if (name == "R_FLOAT")
		return R_FLOAT;
	if (name == "R_INT")
		return R_INT;
	return SEMANTIC_DATATYPE_INVALID;
}

std::string BufferAccessMode::toString(BufferAccessMode::Name name)
{
	if (name == READ_ONLY)
		return "READ_ONLY";
	if (name == WRITE_ONLY)
		return "WRITE_ONLY";
	if (name == READ_WRITE)
		return "READ_WRITE";
	return "";
}

BufferAccessMode::Name BufferAccessMode::fromString(std::string const& name)
{
	if (name == "READ_ONLY")
		return READ_ONLY;
	if (name == "WRITE_ONLY")
		return WRITE_ONLY;
	if (name == "READ_WRITE")
		return READ_WRITE;
	return BUFFER_ACCESS_MODE_INVALID;
}

std::string BufferUsage::toString(BufferUsage::Name name)
{
	if (name == STREAM_DRAW)
		return "STREAM_DRAW";
	if (name == STREAM_READ)
		return "STREAM_READ";
	if (name == STREAM_COPY)
		return "STREAM_COPY";
	if (name == STATIC_DRAW)
		return "STATIC_DRAW";
	if (name == STATIC_READ)
		return "STATIC_READ";
	if (name == STATIC_COPY)
		return "STATIC_COPY";
	if (name == DYNAMIC_DRAW)
		return "DYNAMIC_DRAW";
	if (name == DYNAMIC_READ)
		return "DYNAMIC_READ";
	if (name == DYNAMIC_COPY)
		return "DYNAMIC_COPY";
	return "";
}

BufferUsage::Name BufferUsage::fromString(std::string const& name)
{
	if (name == "STREAM_DRAW")
		return STREAM_DRAW;
	if (name == "STREAM_READ")
		return STREAM_READ;
	if (name == "STREAM_COPY")
		return STREAM_COPY;
	if (name == "STATIC_DRAW")
		return STATIC_DRAW;
	if (name == "STATIC_READ")
		return STATIC_READ;
	if (name == "STATIC_COPY")
		return STATIC_COPY;
	if (name == "DYNAMIC_DRAW")
		return DYNAMIC_DRAW;
	if (name == "DYNAMIC_READ")
		return DYNAMIC_READ;
	if (name == "DYNAMIC_COPY")
		return DYNAMIC_COPY;
	return BUFFER_USAGE_INVALID;
}

std::string DrawMode::toString(DrawMode::Name name)
{
	if (name == POINTS)
		return "POINTS";
	if (name == LINE_STRIP)
		return "LINE_STRIP";
	if (name == LINE_LOOP)
		return "LINE_LOOP";
	if (name == LINES)
		return "LINES";
	if (name == TRIANGLE_STRIP)
		return "TRIANGLE_STRIP";
	if (name == TRIANGLE_FAN)
		return "TRIANGLE_FAN";
	if (name == TRIANGLES)
		return "TRIANGLES";
	if (name == QUADS)
		return "QUADS";
	return "";
}

DrawMode::Name DrawMode::fromString(std::string const& name)
{
	if (name == "POINTS")
		return POINTS;
	if (name == "LINE_STRIP")
		return LINE_STRIP;
	if (name == "LINE_LOOP")
		return LINE_LOOP;
	if (name == "LINES")
		return LINES;
	if (name == "TRIANGLE_STRIP")
		return TRIANGLE_STRIP;
	if (name == "TRIANGLE_FAN")
		return TRIANGLE_FAN;
	if (name == "TRIANGLES")
		return TRIANGLES;
	if (name == "QUADS")
		return QUADS;
	return DRAW_MODE_INVALID;
}

std::string DataType::toString(DataType::Name name)
{
	if (name == BYTE)
		return "BYTE";
	if (name == UNSIGNED_BYTE)
		return "UNSIGNED_BYTE";
	if (name == UNSIGNED_SHORT)
		return "UNSIGNED_SHORT";
	if (name == SHORT)
		return "SHORT";
	if (name == UNSIGNED_INT)
		return "UNSIGNED_INT";
	if (name == INT)
		return "INT";
	if (name == FLOAT)
		return "FLOAT";
	return "";
}

DataType::Name DataType::fromString(std::string const& name)
{
	if (name == "BYTE")
		return BYTE;
	if (name == "UNSIGNED_BYTE")
		return UNSIGNED_BYTE;
	if (name == "UNSIGNED_SHORT")
		return UNSIGNED_SHORT;
	if (name == "SHORT")
		return SHORT;
	if (name == "UNSIGNED_INT")
		return UNSIGNED_INT;
	if (name == "INT")
		return INT;
	if (name == "FLOAT")
		return FLOAT;
	return DATA_TYPE_INVALID;
}

std::string DataFormat::Base::toString(DataFormat::Base::Name name)
{
	if (name == UNKNOWN)
		return "UNKNOWN";
	if (name == RED)
		return "RED";
	if (name == RG)
		return "RG";
	if (name == RGB)
		return "RGB";
	if (name == RGBA)
		return "RGBA";
	if (name == LUMINANCE)
		return "LUMINANCE";
	if (name == DEPTH)
		return "DEPTH";
	return "";
}

DataFormat::Base::Name DataFormat::Base::fromString(std::string const& name)
{
	if (name == "UNKNOWN")
		return UNKNOWN;
	if (name == "RED")
		return RED;
	if (name == "RG")
		return RG;
	if (name == "RGB")
		return RGB;
	if (name == "RGBA")
		return RGBA;
	if (name == "LUMINANCE")
		return LUMINANCE;
	if (name == "DEPTH")
		return DEPTH;
	return DATA_FORMAT_BASE_INVALID;
}

std::string DataFormat::Internal::toString(DataFormat::Internal::Name name)
{
	if (name == UNKNOWN)
		return "UNKNOWN";
	if (name == R8_UI)
		return "R8_UI";
	if (name == R8_I)
		return "RGR8_I";
	if (name == R16_UI)
		return "R16_UI";
	if (name == R16_I)
		return "R16_I";
	if (name == R16_F)
		return "R16_F";
	if (name == R32_UI)
		return "R32_UI";
	if (name == R32_I)
		return "R32_I";
	if (name == R32_F)
		return "R32_F";
	if (name == R8G8_UI)
		return "R8G8_UI";
	if (name == R8G8_I)
		return "R8G8_I";
	if (name == R16G16_UI)
		return "R16G16_UI";
	if (name == R16G16_I)
		return "R16G16_I";
	if (name == R16G16_F)
		return "R16G16_F";
	if (name == R32G32_UI)
		return "R32G32_UI";
	if (name == R32G32_I)
		return "R32G32_I";
	if (name == R32G32_F)
		return "DEPTHR32G32_F";
	if (name == R32G32B32_UI)
		return "R32G32B32_UI";
	if (name == R32G32B32_I)
		return "R32G32B32_I";
	if (name == R32G32B32_F)
		return "R32G32B32_F";
	if (name == R32G32B32A32_UI)
		return "R32G32B32A32_UI";
	if (name == R32G32B32A32_I)
		return "R32G32B32A32_I";
	if (name == R32G32B32A32_F)
		return "R32G32B32A32_F";
	if (name == D16_F)
		return "D16_F";
	if (name == D24_F)
		return "D24_F";
	if (name == D32_F)
		return "D32_F";
	if (name == LUMINANCE_F)
		return "LUMINANCE_F";
	return "";
}

DataFormat::Internal::Name DataFormat::Internal::fromString(std::string const& name)
{
	if (name == "UNKNOWN")
		return UNKNOWN;
	if (name == "R8_UI")
		return R8_UI;
	if (name == "R8_I")
		return R8_I;
	if (name == "R16_UI")
		return R16_UI;
	if (name == "R16_I")
		return R16_I;
	if (name == "R16_F")
		return R16_F;
	if (name == "R32_UI")
		return R32_UI;
	if (name == "R32_I")
		return R32_I;
	if (name == "R32_F")
		return R32_F;
	if (name == "R8G8_UI")
		return R8G8_UI;
	if (name == "R8G8_I")
		return R8G8_I;
	if (name == "R16G16_UI")
		return R16G16_UI;
	if (name == "R16G16_I")
		return R16G16_I;
	if (name == "R16G16_F")
		return R16G16_F;
	if (name == "R32G32_UI")
		return R32G32_UI;
	if (name == "R32G32_I")
		return R32G32_I;
	if (name == "R32G32_F")
		return R32G32_F;
	if (name == "R32G32B32_UI")
		return R32G32B32_UI;
	if (name == "R32G32B32_I")
		return R32G32B32_I;
	if (name == "R32G32B32_F")
		return R32G32B32_F;
	if (name == "R32G32B32A32_UI")
		return R32G32B32A32_UI;
	if (name == "R32G32B32A32_I")
		return R32G32B32A32_I;
	if (name == "R32G32B32A32_F")
		return R32G32B32A32_F;
	if (name == "D16_F")
		return D16_F;
	if (name == "D24_F")
		return D24_F;
	if (name == "D32_F")
		return D32_F;
	if (name == "LUMINANCE_F")
		return LUMINANCE_F;
	return DATA_FORMAT_INTERNAL_INVALID;
}

std::string TextureFormat::toString(TextureFormat::Name name)
{
	if (name == TEXTURE_1D)
		return "TEXTURE_1D";
	if (name == TEXTURE_2D)
		return "TEXTURE_2D";
	if (name == TEXTURE_2D_ARRAY)
		return "TEXTURE_2D_ARRAY";
	if (name == TEXTURE_3D)
		return "TEXTURE_3D";
	if (name == TEXTURE_CUBE_MAP)
		return "TEXTURE_CUBE_MAP";
	if (name == CUBE_MAP_POS_X)
		return "CUBE_MAP_POS_X";
	if (name == CUBE_MAP_NEG_X)
		return "CUBE_MAP_NEG_X";
	if (name == CUBE_MAP_POS_Y)
		return "CUBE_MAP_POS_Y";
	if (name == CUBE_MAP_NEG_Y)
		return "CUBE_MAP_NEG_Y";
	if (name == CUBE_MAP_POS_Z)
		return "CUBE_MAP_POS_Z";
	if (name == CUBE_MAP_NEG_Z)
		return "CUBE_MAP_NEG_Z";
	return "";
}

TextureFormat::Name TextureFormat::fromString(std::string const& name)
{
	if (name == "TEXTURE_1D")
		return TEXTURE_1D;
	if (name == "TEXTURE_2D")
		return TEXTURE_2D;
	if (name == "TEXTURE_2D_ARRAY")
		return TEXTURE_2D_ARRAY;
	if (name == "TEXTURE_3D")
		return TEXTURE_3D;
	if (name == "TEXTURE_CUBE_MAP")
		return TEXTURE_CUBE_MAP;
	if (name == "CUBE_MAP_POS_X")
		return CUBE_MAP_POS_X;
	if (name == "CUBE_MAP_NEG_X")
		return CUBE_MAP_NEG_X;
	if (name == "CUBE_MAP_POS_Y")
		return CUBE_MAP_POS_Y;
	if (name == "CUBE_MAP_NEG_Y")
		return CUBE_MAP_NEG_Y;
	if (name == "CUBE_MAP_POS_Z")
		return CUBE_MAP_POS_Z;
	if (name == "CUBE_MAP_NEG_Z")
		return CUBE_MAP_NEG_Z;
	return TEXTURE_FORMAT_INVALID;
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

std::string FrameBufferAttachmentPoint::toString(FrameBufferAttachmentPoint::Name name)
{
	if (name == COLOR_0)
		return "COLOR_0";
	if (name == COLOR_1)
		return "COLOR_1";
	if (name == COLOR_2)
		return "COLOR_2";
	if (name == COLOR_3)
		return "COLOR_3";
	if (name == COLOR_4)
		return "COLOR_4";
	if (name == COLOR_5)
		return "COLOR_5";
	if (name == COLOR_6)
		return "COLOR_6";
	if (name == COLOR_7)
		return "COLOR_7";
	if (name == COLOR_8)
		return "COLOR_8";
	if (name == COLOR_9)
		return "COLOR_9";
	if (name == COLOR_10)
		return "COLOR_10";
	if (name == COLOR_11)
		return "COLOR_11";
	if (name == COLOR_12)
		return "COLOR_12";
	if (name == COLOR_13)
		return "COLOR_13";
	if (name == COLOR_14)
		return "COLOR_14";
	if (name == COLOR_15)
		return "COLOR_15";
	if (name == DEPTH)
		return "DEPTH";
	if (name == STENCIL)
		return "STENCIL";
	return "";
}

FrameBufferAttachmentPoint::Name FrameBufferAttachmentPoint::fromString(std::string const& name)
{
	if (name == "COLOR_0")
		return COLOR_0;
	if (name == "COLOR_1")
		return COLOR_1;
	if (name == "COLOR_2")
		return COLOR_2;
	if (name == "COLOR_3")
		return COLOR_3;
	if (name == "COLOR_4")
		return COLOR_4;
	if (name == "COLOR_5")
		return COLOR_5;
	if (name == "COLOR_6")
		return COLOR_6;
	if (name == "COLOR_7")
		return COLOR_7;
	if (name == "COLOR_8")
		return COLOR_8;
	if (name == "COLOR_9")
		return COLOR_9;
	if (name == "COLOR_10")
		return COLOR_10;
	if (name == "COLOR_11")
		return COLOR_11;
	if (name == "COLOR_12")
		return COLOR_12;
	if (name == "COLOR_13")
		return COLOR_13;
	if (name == "COLOR_14")
		return COLOR_14;
	if (name == "COLOR_15")
		return COLOR_15;
	if (name == "DEPTH")
		return DEPTH;
	if (name == "STENCIL")
		return STENCIL;
	return ATTACHMENT_INVALID;
}

std::string WrapMode::toString(WrapMode::Name name)
{
	if (name == CLAMP_TO_EDGE)
		return "CLAMP_TO_EDGE";
	if (name == CLAMP_TO_BORDER)
		return "CLAMP_TO_BORDER";
	if (name == MIRROR_CLAMP_TO_EDGE)
		return "MIRROR_CLAMP_TO_EDGE";
	if (name == MIRRORED_REPEAT)
		return "MIRRORED_REPEAT";
	return "";
}

WrapMode::Name WrapMode::fromString(std::string const& name)
{
	if(name == "CLAMP_TO_EDGE") 
		return CLAMP_TO_EDGE;
	if(name == "CLAMP_TO_BORDER") 
		return CLAMP_TO_BORDER;
	if(name == "MIRRORED_REPEAT") 
		return MIRRORED_REPEAT;
	if(name == "MIRROR_CLAMP_TO_EDGE") 
		return MIRROR_CLAMP_TO_EDGE;
	if(name == "REPEAT") 
		return REPEAT;
	return WRAP_MODE_INVALID;
}

std::string FilterMode::toString(FilterMode::Name name)
{
	if (name == NEAREST)
		return "NEAREST";
	if (name == LINEAR)
		return "LINEAR";
	if (name == NEAREST_MIPMAP_NEAREST)
		return "NEAREST_MIPMAP_NEAREST";
	if (name == LINEAR_MIPMAP_NEAREST)
		return "LINEAR_MIPMAP_NEAREST";
	if (name == NEAREST_MIPMAP_LINEAR)
		return "NEAREST_MIPMAP_LINEAR";
	if (name == LINEAR_MIPMAP_LINEAR)
		return "LINEAR_MIPMAP_LINEAR";
	return "";
}

FilterMode::Name FilterMode::fromString(std::string const& name)
{
	if (name == "NEAREST")
		return NEAREST;
	if (name == "LINEAR")
		return LINEAR;
	if (name == "NEAREST_MIPMAP_NEAREST")
		return NEAREST_MIPMAP_NEAREST;
	if (name == "LINEAR_MIPMAP_NEAREST")
		return LINEAR_MIPMAP_NEAREST;
	if (name == "NEAREST_MIPMAP_LINEAR")
		return NEAREST_MIPMAP_LINEAR;
	if (name == "LINEAR_MIPMAP_LINEAR")
		return LINEAR_MIPMAP_LINEAR;
	return FILTER_MODE_INVALID;
}

std::string TexSlot::toString(TexSlot::Name name)
{
	if (name == TEX_SLOT1)
		return "TEX_SLOT1";
	if (name == TEX_SLOT2)
		return "TEX_SLOT2";
	if (name == TEX_SLOT3)
		return "TEX_SLOT3";
	if (name == TEX_SLOT4)
		return "TEX_SLOT4";
	if (name == TEX_SLOT5)
		return "TEX_SLOT5";
	if (name == TEX_SLOT6)
		return "TEX_SLOT6";
	if (name == TEX_SLOT7)
		return "TEX_SLOT7";
	if (name == TEX_SLOT8)
		return "TEX_SLOT8";
	if (name == TEX_SLOT9)
		return "TEX_SLOT9";
	if (name == TEX_SLOT10)
		return "TEX_SLOT10";
	if (name == TEX_SLOT11)
		return "TEX_SLOT11";
	if (name == TEX_SLOT12)
		return "TEX_SLOT12";
	if (name == TEX_SLOT13)
		return "TEX_SLOT13";
	if (name == TEX_SLOT14)
		return "TEX_SLOT14";
	if (name == TEX_SLOT15)
		return "TEX_SLOT15";
	if (name == TEX_SLOT16)
		return "TEX_SLOT16";
	if (name == TEX_SLOT17)
		return "TEX_SLOT17";
	if (name == TEX_SLOT18)
		return "TEX_SLOT18";
	if (name == TEX_SLOT19)
		return "TEX_SLOT19";
	if (name == TEX_SLOT20)
		return "TEX_SLOT20";
	if (name == TEX_SLOT21)
		return "TEX_SLOT21";
	if (name == TEX_SLOT22)
		return "TEX_SLOT22";
	if (name == TEX_SLOT23)
		return "TEX_SLOT23";
	if (name == TEX_SLOT24)
		return "TEX_SLOT24";
	if (name == TEX_SLOT25)
		return "TEX_SLOT25";
	if (name == TEX_SLOT26)
		return "TEX_SLOT26";
	if (name == TEX_SLOT27)
		return "TEX_SLOT27";
	if (name == TEX_SLOT28)
		return "TEX_SLOT28";
	if (name == TEX_SLOT29)
		return "TEX_SLOT29";
	if (name == TEX_SLOT30)
		return "TEX_SLOT30";
	if (name == TEX_SLOT31)
		return "TEX_SLOT31";
	if (name == TEX_SLOT32)
		return "TEX_SLOT32";
	return "";
}

TexSlot::Name TexSlot::fromString(std::string const& name)
{
	if (name == "TEX_SLOT1")
		return TEX_SLOT1;
	if (name == "TEX_SLOT2")
		return TEX_SLOT2;
	if (name == "TEX_SLOT3")
		return TEX_SLOT3;
	if (name == "TEX_SLOT4")
		return TEX_SLOT4;
	if (name == "TEX_SLOT5")
		return TEX_SLOT5;
	if (name == "TEX_SLOT6")
		return TEX_SLOT6;
	if (name == "TEX_SLOT7")
		return TEX_SLOT7;
	if (name == "TEX_SLOT8")
		return TEX_SLOT8;
	if (name == "TEX_SLOT9")
		return TEX_SLOT9;
	if (name == "TEX_SLOT10")
		return TEX_SLOT10;
	if (name == "TEX_SLOT11")
		return TEX_SLOT11;
	if (name == "TEX_SLOT12")
		return TEX_SLOT12;
	if (name == "TEX_SLOT13")
		return TEX_SLOT13;
	if (name == "TEX_SLOT14")
		return TEX_SLOT14;
	if (name == "TEX_SLOT15")
		return TEX_SLOT15;
	if (name == "TEX_SLOT16")
		return TEX_SLOT16;
	if (name == "TEX_SLOT17")
		return TEX_SLOT17;
	if (name == "TEX_SLOT18")
		return TEX_SLOT18;
	if (name == "TEX_SLOT19")
		return TEX_SLOT19;
	if (name == "TEX_SLOT20")
		return TEX_SLOT20;
	if (name == "TEX_SLOT21")
		return TEX_SLOT21;
	if (name == "TEX_SLOT22")
		return TEX_SLOT22;
	if (name == "TEX_SLOT23")
		return TEX_SLOT23;
	if (name == "TEX_SLOT24")
		return TEX_SLOT24;
	if (name == "TEX_SLOT25")
		return TEX_SLOT25;
	if (name == "TEX_SLOT26")
		return TEX_SLOT26;
	if (name == "TEX_SLOT27")
		return TEX_SLOT27;
	if (name == "TEX_SLOT28")
		return TEX_SLOT28;
	if (name == "TEX_SLOT29")
		return TEX_SLOT29;
	if (name == "TEX_SLOT30")
		return TEX_SLOT30;
	if (name == "TEX_SLOT31")
		return TEX_SLOT31;
	if (name == "TEX_SLOT32")
		return TEX_SLOT32;
	return TEX_SLOT_INVALID;
}


std::string FaceCulling::toString(FaceCulling::Name name)
{
	if (name == BACK_FACE)
		return "BACK_FACE";
	if (name == FRONT_FACE)
		return "FRONT_FACE";
	if (name == FRONT_AND_BACK_FACE)
		return "FRONT_AND_BACK_FACE";
	return "";
}

FaceCulling::Name FaceCulling::fromString(std::string const& name)
{
	if (name == "BACK_FACE")
		return BACK_FACE;
	if (name == "FRONT_FACE")
		return FRONT_FACE;
	if (name == "FRONT_AND_BACK_FACE")
		return FRONT_AND_BACK_FACE;
	return FACE_CULLING_INVALID;
}

std::string PolygonDrawMode::toString(PolygonDrawMode::Name name)
{
	if (name == FILL)
		return "FILL";
	if (name == LINE)
		return "LINE";
	if (name == POINT)
		return "POINT";
	return "";
}

PolygonDrawMode::Name PolygonDrawMode::fromString(std::string const& name)
{
	if (name == "FILL")
		return FILL;
	if (name == "LINE")
		return LINE;
	if (name == "POINT")
		return POINT;
	return POLYGON_DRAW_MODE_INVALID;
}

std::string BlendFactor::toString(BlendFactor::Name name)
{
	if (name == ONE)
		return "ONE";
	if (name == ZERO)
		return "ZERO";
	if (name == SRC_COLOR)
		return "SRC_COLOR";
	if (name == ONE_MINUS_SRC_COLOR)
		return "ONE_MINUS_SRC_COLOR";
	if (name == DST_COLOR)
		return "DST_COLOR";
	if (name == ONE_MINUS_DST_COLOR)
		return "ONE_MINUS_DST_COLOR";
	if (name == SRC_ALPHA)
		return "SRC_ALPHA";
	if (name == ONE_MINUS_SRC_ALPHA)
		return "ONE_MINUS_SRC_ALPHA";
	if (name == DST_ALPHA)
		return "DST_ALPHA";
	if (name == ONE_MINUS_DST_ALPHA)
		return "ONE_MINUS_DST_ALPHA";
	if (name == CONSTANT_COLOR)
		return "CONSTANT_COLOR";
	if (name == ONE_MINUS_CONSTANT_COLOR)
		return "ONE_MINUS_CONSTANT_COLOR";
	if (name == CONSTANT_ALPHA)
		return "CONSTANT_ALPHA";
	if (name == ONE_MINUS_CONSTANT_ALPHA)
		return "ONE_MINUS_CONSTANT_ALPHA";
	if (name == SRC_ALPHA_SATURATE)
		return "SRC_ALPHA_SATURATE";
	if (name == SRC1_COLOR)
		return "SRC1_COLOR";
	if (name == ONE_MINUS_SRC1_COLOR)
		return "ONE_MINUS_SRC1_COLOR";
	if (name == SRC1_ALPHA)
		return "SRC1_ALPHA";
	if (name == ONE_MINUS_SRC1_ALPHA)
		return "ONE_MINUS_SRC1_ALPHA";
	return "";
}

BlendFactor::Name BlendFactor::fromString(std::string const& name)
{
	if (name == "ONE")
		return ONE;
	if (name == "ZERO")
		return ZERO;
	if (name == "SRC_COLOR")
		return SRC_COLOR;
	if (name == "ONE_MINUS_SRC_COLOR")
		return ONE_MINUS_SRC_COLOR;
	if (name == "DST_COLOR")
		return DST_COLOR;
	if (name == "ONE_MINUS_DST_COLOR")
		return ONE_MINUS_DST_COLOR;
	if (name == "SRC_ALPHA")
		return SRC_ALPHA;
	if (name == "ONE_MINUS_SRC_ALPHA")
		return ONE_MINUS_SRC_ALPHA;
	if (name == "DST_ALPHA")
		return DST_ALPHA;
	if (name == "ONE_MINUS_DST_ALPHA")
		return ONE_MINUS_DST_ALPHA;
	if (name == "CONSTANT_COLOR")
		return CONSTANT_COLOR;
	if (name == "ONE_MINUS_CONSTANT_COLOR")
		return ONE_MINUS_CONSTANT_COLOR;
	if (name == "CONSTANT_ALPHA")
		return CONSTANT_ALPHA;
	if (name == "ONE_MINUS_CONSTANT_ALPHA")
		return ONE_MINUS_CONSTANT_ALPHA;
	if (name == "SRC_ALPHA_SATURATE")
		return SRC_ALPHA_SATURATE;
	if (name == "SRC1_COLOR")
		return SRC1_COLOR;
	if (name == "ONE_MINUS_SRC1_COLOR")
		return ONE_MINUS_SRC1_COLOR;
	if (name == "SRC1_ALPHA")
		return SRC1_ALPHA;
	if (name == "ONE_MINUS_SRC1_ALPHA")
		return ONE_MINUS_SRC1_ALPHA;
	return BLEND_FACTOR_INVALID;
}

std::string DepthFunc::toString(DepthFunc::Name name)
{
	if (name == NEVER)
		return "NEVER";
	if (name == LESS)
		return "LESS";
	if (name == EQUAL)
		return "EQUAL";
	if (name == LEQUAL)
		return "LEQUAL";
	if (name == GREATER)
		return "GREATER";
	if (name == NOT_EQUAL)
		return "NOT_EQUAL";
	if (name == GEQUAL)
		return "GEQUAL";
	if (name == ALWAYS)
		return "ALWAYS";
	return "";
}

DepthFunc::Name DepthFunc::fromString(std::string const& name)
{
	if (name == "NEVER")
		return NEVER;
	if (name == "LESS")
		return LESS;
	if (name == "EQUAL")
		return EQUAL;
	if (name == "LEQUAL")
		return LEQUAL;
	if (name == "GREATER")
		return GREATER;
	if (name == "NOT_EQUAL")
		return NOT_EQUAL;
	if (name == "GEQUAL")
		return GEQUAL;
	if (name == "ALWAYS")
		return ALWAYS;
	return DEPTH_FUNC_INVALID;
}

RenderLayer::RenderLayerMask
G2Core::RenderLayer::fromString(std::string const& renderLayer)
{
	if (renderLayer == "TERRAIN")
		return TERRAIN;
	if (renderLayer == "WATER")
		return WATER;
	if (renderLayer == "TRANSPARENT_MESH")
		return TRANSPARENT_MESH;
	if (renderLayer == "STATIC_MESH")
		return STATIC_MESH;
	if (renderLayer == "DYNAMIC_MESH")
		return DYNAMIC_MESH;
	if (renderLayer == "LAYER_5")
		return LAYER_5;
	if (renderLayer == "LAYER_6")
		return LAYER_6;
	if (renderLayer == "LAYER_7")
		return LAYER_7;
	if (renderLayer == "LAYER_8")
		return LAYER_8;
	if (renderLayer == "LAYER_9")
		return LAYER_9;
	if (renderLayer == "LAYER_10")
		return LAYER_10;
	if (renderLayer == "LAYER_11")
		return LAYER_11;
	if (renderLayer == "LAYER_12")
		return LAYER_12;
	if (renderLayer == "LAYER_13")
		return LAYER_13;
	if (renderLayer == "LAYER_14")
		return LAYER_14;
	if (renderLayer == "LAYER_15")
		return LAYER_15;
	if (renderLayer == "LAYER_16")
		return LAYER_16;
	if (renderLayer == "LAYER_17")
		return LAYER_17;
	if (renderLayer == "LAYER_18")
		return LAYER_18;
	if (renderLayer == "LAYER_19")
		return LAYER_19;
	if (renderLayer == "LAYER_20")
		return LAYER_20;
	if (renderLayer == "LAYER_21")
		return LAYER_21;
	if (renderLayer == "LAYER_22")
		return LAYER_22;
	if (renderLayer == "LAYER_23")
		return LAYER_23;
	if (renderLayer == "LAYER_24")
		return LAYER_24;
	if (renderLayer == "LAYER_25")
		return LAYER_25;
	if (renderLayer == "LAYER_26")
		return LAYER_26;
	if (renderLayer == "LAYER_27")
		return LAYER_27;
	if (renderLayer == "LAYER_28")
		return LAYER_28;
	if (renderLayer == "LAYER_29")
		return LAYER_29;
	if (renderLayer == "LAYER_30")
		return LAYER_30;
	return LAYER_INVALID;
}

std::string RenderLayer::toString(RenderLayer::Name name)
{
	if (name == TERRAIN)
		return "TERRAIN";
	if (name == WATER)
		return "WATER";
	if (name == TRANSPARENT_MESH)
		return "TRANSPARENT_MESH";
	if (name == STATIC_MESH)
		return "STATIC_MESH";
	if (name == DYNAMIC_MESH)
		return "DYNAMIC_MESH";
	if (name == LAYER_5)
		return "LAYER_5";
	if (name == LAYER_6)
		return "LAYER_6";
	if (name == LAYER_7)
		return "LAYER_7";
	if (name == LAYER_8)
		return "LAYER_8";
	if (name == LAYER_9)
		return "LAYER_9";
	if (name == LAYER_10)
		return "LAYER_10";
	if (name == LAYER_11)
		return "LAYER_11";
	if (name == LAYER_12)
		return "LAYER_12";
	if (name == LAYER_13)
		return "LAYER_13";
	if (name == LAYER_14)
		return "LAYER_14";
	if (name == LAYER_15)
		return "LAYER_15";
	if (name == LAYER_16)
		return "LAYER_16";
	if (name == LAYER_17)
		return "LAYER_17";
	if (name == LAYER_18)
		return "LAYER_18";
	if (name == LAYER_19)
		return "LAYER_19";
	if (name == LAYER_20)
		return "LAYER_20";
	if (name == LAYER_21)
		return "LAYER_21";
	if (name == LAYER_22)
		return "LAYER_22";
	if (name == LAYER_23)
		return "LAYER_23";
	if (name == LAYER_24)
		return "LAYER_24";
	if (name == LAYER_25)
		return "LAYER_25";
	if (name == LAYER_26)
		return "LAYER_26";
	if (name == LAYER_27)
		return "LAYER_27";
	if (name == LAYER_28)
		return "LAYER_28";
	if (name == LAYER_29)
		return "LAYER_29";
	if (name == LAYER_30)
		return "LAYER_30";
	return "";
}

std::string UniformBufferBindingPoint::toString(UniformBufferBindingPoint::Name name)
{
	if (name == UBO_MATERIAL)
		return "UBO_MATERIAL";
	if (name == UBO_LIGHTS)
		return "UBO_LIGHTS";
	if (name == UBO_MATRICES)
		return "UBO_MATRICES";
	if (name == UBO_POST_PROCESSING_INFO)
		return "UBO_POST_PROCESSING_INFO";
	return "";
}

UniformBufferBindingPoint::Name UniformBufferBindingPoint::fromString(std::string const& name)
{
	if (name == "UBO_MATERIAL")
		return UBO_MATERIAL;
	if (name == "UBO_LIGHTS")
		return UBO_LIGHTS;
	if (name == "UBO_MATRICES")
		return UBO_MATRICES;
	if (name == "UBO_POST_PROCESSING_INFO")
		return UBO_POST_PROCESSING_INFO;
	return UNIFORM_BUFFER_BINDING_POINT_INVALID;
}