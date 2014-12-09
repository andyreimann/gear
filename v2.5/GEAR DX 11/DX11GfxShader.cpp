#include "DX11GfxApi.h"
#include "DX11GfxData.h"
#include "DX11GfxMappings.h"

#include <G2/Logger.h>

#include <windowsx.h> 
#include <d3d11.h>		// core
#include <d3dx11.h>		// extensions
#include <d3dx10.h>		// dx 11 is ext of dx 10 -> useful macros usable

G2Core::GfxResource* _setupShaderFunctionPointers(G2Core::GfxResource* res);

G2Core::GfxResource* CompileShader(G2Core::VertexInputLayout const& vertexInputLayout, std::string const& shadingLanguage, std::string const& vertexCode, std::string const& geometryCode, std::string const& fragmentCode)
{
	assert(vertexInputLayout.elements.size() > 0);
	if(shadingLanguage == "HLSL")
	{
		assert(false);	
	}
	return error();
}

void _bindShaderHlsl(G2Core::GfxResource const* shaderResource)
{
	assert(false);
}

void BindShader(G2Core::GfxResource const* shaderResource)
{
	G2DX11::ShaderResource const* res = static_cast<G2DX11::ShaderResource const*>(shaderResource);
	res->bindShader(shaderResource);
}

void _setShaderUniformMat4Hlsl(G2Core::GfxResource* shaderResource, std::string const& property, glm::mat4 const& value)
{
	assert(false);
}


void SetShaderUniformMat4(G2Core::GfxResource* shaderResource, std::string const& property, glm::mat4 const& value)
{
	static_cast<G2DX11::ShaderResource*>(shaderResource)->setShaderUniformMat4(shaderResource, property, value);
}

void _setShaderUniformMat3Hlsl(G2Core::GfxResource* shaderResource, std::string const& property, glm::mat3 const& value)
{
	assert(false);
}

void SetShaderUniformMat3(G2Core::GfxResource* shaderResource, std::string const& property, glm::mat3 const& value)
{
	static_cast<G2DX11::ShaderResource*>(shaderResource)->setShaderUniformMat3(shaderResource, property, value);
}

void _setShaderUniformVec4Hlsl(G2Core::GfxResource* shaderResource, std::string const& property, glm::vec4 const& value)
{
	assert(false);
}

void SetShaderUniformVec4(G2Core::GfxResource* shaderResource, std::string const& property, glm::vec4 const& value)
{
	static_cast<G2DX11::ShaderResource*>(shaderResource)->setShaderUniformVec4(shaderResource, property, value);
}

void _setShaderUniformVec3Hlsl(G2Core::GfxResource* shaderResource, std::string const& property, glm::vec3 const& value)
{
	assert(false);
}

void SetShaderUniformVec3(G2Core::GfxResource* shaderResource, std::string const& property, glm::vec3 const& value)
{
	static_cast<G2DX11::ShaderResource*>(shaderResource)->setShaderUniformVec3(shaderResource, property, value);
}

void _setShaderUniformVec2Hlsl(G2Core::GfxResource* shaderResource, std::string const& property, glm::vec2 const& value)
{
	assert(false);
}

void SetShaderUniformVec2(G2Core::GfxResource* shaderResource, std::string const& property, glm::vec2 const& value)
{
	static_cast<G2DX11::ShaderResource*>(shaderResource)->setShaderUniformVec2(shaderResource, property, value);
}

void _setShaderUniformFloatHlsl(G2Core::GfxResource* shaderResource, std::string const& property, float value)
{
	assert(false);
}

void SetShaderUniformFloat(G2Core::GfxResource* shaderResource, std::string const& property, float value)
{
	static_cast<G2DX11::ShaderResource*>(shaderResource)->setShaderUniformFloat(shaderResource, property, value);
}

void _setShaderUniformIntHlsl(G2Core::GfxResource* shaderResource, std::string const& property, int value)
{
	assert(false);
}

void SetShaderUniformInt(G2Core::GfxResource* shaderResource, std::string const& property, int value)
{
	static_cast<G2DX11::ShaderResource*>(shaderResource)->setShaderUniformInt(shaderResource, property, value);
}

G2Core::GfxResource* _setupShaderFunctionPointers(G2Core::GfxResource* res)
{
	G2DX11::ShaderResource* r = static_cast<G2DX11::ShaderResource*>(res);
	switch(type(res))
	{
	case G2DX11::HLSL_SHADER:
		r->bindShader = _bindShaderHlsl;
		r->setShaderUniformMat4 = _setShaderUniformMat4Hlsl;
		r->setShaderUniformMat3 = _setShaderUniformMat3Hlsl;
		r->setShaderUniformVec4 = _setShaderUniformVec4Hlsl;
		r->setShaderUniformVec3 = _setShaderUniformVec3Hlsl;
		r->setShaderUniformVec2 = _setShaderUniformVec2Hlsl;
		r->setShaderUniformFloat = _setShaderUniformFloatHlsl;
		r->setShaderUniformInt = _setShaderUniformIntHlsl;
		break;
	default:
		r->bindShader = nullptr;
		r->setShaderUniformMat4 = nullptr;
		r->setShaderUniformMat3 = nullptr;
		r->setShaderUniformVec4 = nullptr;
		r->setShaderUniformVec3 = nullptr;
		r->setShaderUniformVec2 = nullptr;
		r->setShaderUniformFloat = nullptr;
		r->setShaderUniformInt = nullptr;
		break;
	}
	return res;
}