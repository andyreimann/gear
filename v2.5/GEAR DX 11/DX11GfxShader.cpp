#include "DX11GfxApi.h"
#include "DX11GfxData.h"
#include "DX11GfxMappings.h"

#include <G2/Logger.h>

#include <windowsx.h> 
#include <d3d11.h>		// core
#include <d3dx11.h>		// extensions
#include <d3dx10.h>		// dx 11 is ext of dx 10 -> useful macros usable

CGcontext gCgContext = nullptr;

CGprofile CG_VERTEX_SHADER_PROFILES[] = {
	CG_PROFILE_VS_5_0,
	CG_PROFILE_VS_4_0,
	CG_PROFILE_VS_3_0,
	CG_PROFILE_VS_2_0,
	CG_PROFILE_VS_1_1
};

CGprofile CG_GEOMETRY_SHADER_PROFILES[] = {
	CG_PROFILE_GS_5_0,
	CG_PROFILE_GS_4_0
};

CGprofile CG_PIXEL_SHADER_PROFILES[] = {
	CG_PROFILE_PS_5_0,
	CG_PROFILE_PS_4_0,
	CG_PROFILE_PS_3_0,
	CG_PROFILE_PS_2_0,
	CG_PROFILE_PS_1_1
};

void _cgErrorHandler(CGcontext context, CGerror error, void* appData) 
{
	if(error != CG_NO_ERROR)
	{
		const char* strPtr = cgGetErrorString(error);
		G2::logger << "[CgRuntime] Cg Error: \n" << std::string(strPtr != nullptr ? strPtr : "");
		
		if(error == CG_COMPILER_ERROR)
		{
			strPtr = cgGetLastListing(context);
			G2::logger << std::string(strPtr != nullptr ? strPtr : "");
		}
		G2::logger << G2::endl;
	}
}

void _initCgRuntime(ID3D11Device* device)
{
	if(gCgContext == nullptr) {
		G2::logger << "[CgRuntime] Initialize Cg-Runtime-DirectX 11" << G2::endl;
		//qDebug("[Cg] Initialize Cg");
		// register the error handler
		cgSetErrorHandler( &_cgErrorHandler, NULL);
		// create a new Cg Context
		gCgContext = cgCreateContext();

		HRESULT hr = cgD3D11SetDevice( gCgContext, device);
	
		if( hr != S_OK )
			return;

		//// Register the default state assignment for OpenGL
		//cgD3D11RegisterStates(gCgContext);
		//// This will allow the Cg runtime to manage texture binding
		//cgD3D11SetManageTextureParameters(gCgContext, CG_TRUE);
	}
}

G2Core::GfxResource* _setupShaderFunctionPointers(G2Core::GfxResource* res);

G2Core::GfxResource* CompileShader(G2Core::VertexInputLayout const& vertexInputLayout, std::string const& shadingLanguage, std::string const& vertexCode, std::string const& geometryCode, std::string const& fragmentCode)
{
	assert(vertexInputLayout.elements.size() > 0);
	if(shadingLanguage == "HLSL")
	{
		assert(false);	
	}
	else if(shadingLanguage == "CG")
	{
		HRESULT hr;
		const char* sourcePtr = vertexCode.c_str();
	
		int D3DCOMPILE_OPTIMIZATION_LEVEL3 = (1 << 15);

		int flags = D3DCOMPILE_OPTIMIZATION_LEVEL3;

		CGprogram vertexShaderId = nullptr;
		int num = sizeof(CG_VERTEX_SHADER_PROFILES) / sizeof(CG_VERTEX_SHADER_PROFILES[0]);
		for (int i = 0; i < num; ++i)
		{
			auto optimal = cgD3D11GetOptimalOptions(CG_VERTEX_SHADER_PROFILES[i]);
			vertexShaderId = cgCreateProgram(gCgContext, CG_SOURCE, sourcePtr, CG_VERTEX_SHADER_PROFILES[i], "main", optimal);
			hr = cgD3D11LoadProgram(vertexShaderId, flags);

			if (SUCCEEDED(hr))
				break;
		}
		if (!SUCCEEDED(hr) || vertexShaderId == nullptr)
		{
			G2::logger << "[Cg] Could not find any profile to compile vertex shader with!" << G2::endl;
			return error();
		}


		CGprogram geometryShaderId = nullptr;
		if (geometryCode != "")
		{
			sourcePtr = geometryCode.c_str();
			num = sizeof(CG_GEOMETRY_SHADER_PROFILES) / sizeof(CG_GEOMETRY_SHADER_PROFILES[0]);
			for (int i = 0; i < num; ++i)
			{
				auto optimal = cgD3D11GetOptimalOptions(CG_GEOMETRY_SHADER_PROFILES[i]);
				geometryShaderId = cgCreateProgram(gCgContext, CG_SOURCE, sourcePtr, CG_GEOMETRY_SHADER_PROFILES[i], "main", optimal);
				hr = cgD3D11LoadProgram(geometryShaderId, flags);

				if (SUCCEEDED(hr))
					break;
			}
			if (!SUCCEEDED(hr) || geometryShaderId == nullptr)
			{
				G2::logger << "[Cg] Could not find any profile to compile geometry shader with!" << G2::endl;
				return error();
			}
		}


		CGprogram fragmentShaderId = nullptr;
		num = sizeof(CG_PIXEL_SHADER_PROFILES) / sizeof(CG_PIXEL_SHADER_PROFILES[0]);
		sourcePtr = fragmentCode.c_str();
		for (int i = 0; i < num; ++i)
		{
			auto optimal = cgD3D11GetOptimalOptions(CG_PIXEL_SHADER_PROFILES[i]);
			fragmentShaderId = cgCreateProgram(gCgContext, CG_SOURCE, sourcePtr, CG_PIXEL_SHADER_PROFILES[i], "main", optimal);
			hr = cgD3D11LoadProgram(fragmentShaderId, flags);

			if (SUCCEEDED(hr))
				break;
		}
		if (!SUCCEEDED(hr) || fragmentShaderId == nullptr)
		{
			G2::logger << "[Cg] Could not find any profile to compile pixel shader with!" << G2::endl;
			return error();
		}

		G2DX11::InputLayoutShaderFragment inputLayoutShaderFragment;
		for(int i = 0; i < vertexInputLayout.elements.size(); ++i)
		{
			inputLayoutShaderFragment.semantics.push_back(vertexInputLayout.elements[i].semantic);
			inputLayoutShaderFragment.semanticNames.push_back(toD3DSemanticString(vertexInputLayout.elements[i].semantic));
		}

		G2DX11::CgShaderResource* resource = new G2DX11::CgShaderResource(cgD3D11GetCompiledProgram(vertexShaderId),inputLayoutShaderFragment);
		resource->vertexShaderId = vertexShaderId;
		resource->geometryShaderId = geometryShaderId;
		resource->fragmentShaderId = fragmentShaderId;
		resource->valid = vertexShaderId != nullptr && fragmentShaderId != nullptr;
		return _setupShaderFunctionPointers(resource);
	}
	return error();
}

void _bindShaderHlsl(G2Core::GfxResource const* shaderResource)
{
	assert(false);
}

void _bindShaderCg(G2Core::GfxResource const* shaderResource)
{
	G2DX11::CgShaderResource const* cgRes = static_cast<G2DX11::CgShaderResource const*>(shaderResource);
	
	HRESULT hr;
	if(cgRes->geometryShaderId != nullptr)
	{
		hr = cgD3D11BindProgram(cgRes->geometryShaderId);
	}
	hr = cgD3D11BindProgram(cgRes->vertexShaderId);
	if (!SUCCEEDED(hr))
	{
		G2::logger << "Could not bind Vertex Shader" << G2::endl;
	}
	hr = cgD3D11BindProgram(cgRes->fragmentShaderId);
	if (!SUCCEEDED(hr))
	{
		G2::logger << "Could not bind Fragment Shader" << G2::endl;
	}
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

void _setShaderUniformMat4Cg(G2Core::GfxResource* shaderResource, std::string const& property, glm::mat4 const& value)
{
	G2DX11::CgShaderResource* cgRes = static_cast<G2DX11::CgShaderResource*>(shaderResource);
	auto location = cgRes->getAndCacheUniformLocation(property);
	if(location.first == nullptr) 
	{
		return;
	}
	cgSetMatrixParameterfc(location.first, glm::value_ptr(value));
	cgRes->bindShader(shaderResource);
}


void SetShaderUniformMat4(G2Core::GfxResource* shaderResource, std::string const& property, glm::mat4 const& value)
{
	static_cast<G2DX11::ShaderResource*>(shaderResource)->setShaderUniformMat4(shaderResource, property, value);
}

void _setShaderUniformMat3Hlsl(G2Core::GfxResource* shaderResource, std::string const& property, glm::mat3 const& value)
{
	assert(false);
}


void _setShaderUniformMat3Cg(G2Core::GfxResource* shaderResource, std::string const& property, glm::mat3 const& value)
{
	G2DX11::CgShaderResource* cgRes = static_cast<G2DX11::CgShaderResource*>(shaderResource);
	auto location = cgRes->getAndCacheUniformLocation(property);
	if(location.first == nullptr) 
	{
		return;
	}
	cgSetMatrixParameterfc(location.first, glm::value_ptr(value));
	cgRes->bindShader(shaderResource);
}

void SetShaderUniformMat3(G2Core::GfxResource* shaderResource, std::string const& property, glm::mat3 const& value)
{
	static_cast<G2DX11::ShaderResource*>(shaderResource)->setShaderUniformMat3(shaderResource, property, value);
}

void _setShaderUniformVec4Hlsl(G2Core::GfxResource* shaderResource, std::string const& property, glm::vec4 const& value)
{
	assert(false);
}


void _setShaderUniformVec4Cg(G2Core::GfxResource* shaderResource, std::string const& property, glm::vec4 const& value)
{
	G2DX11::CgShaderResource* cgRes = static_cast<G2DX11::CgShaderResource*>(shaderResource);
	auto location = cgRes->getAndCacheUniformLocation(property);
	if(location.first == nullptr) 
	{
		return;
	}
	cgSetParameter4fv(location.first, glm::value_ptr(value));
	cgRes->bindShader(shaderResource);
}

void SetShaderUniformVec4(G2Core::GfxResource* shaderResource, std::string const& property, glm::vec4 const& value)
{
	static_cast<G2DX11::ShaderResource*>(shaderResource)->setShaderUniformVec4(shaderResource, property, value);
}

void _setShaderUniformVec3Hlsl(G2Core::GfxResource* shaderResource, std::string const& property, glm::vec3 const& value)
{
	assert(false);
}


void _setShaderUniformVec3Cg(G2Core::GfxResource* shaderResource, std::string const& property, glm::vec3 const& value)
{
	G2DX11::CgShaderResource* cgRes = static_cast<G2DX11::CgShaderResource*>(shaderResource);
	auto location = cgRes->getAndCacheUniformLocation(property);
	if(location.first == nullptr) 
	{
		return;
	}
	cgSetParameter3fv(location.first, glm::value_ptr(value));
	cgRes->bindShader(shaderResource);
}

void SetShaderUniformVec3(G2Core::GfxResource* shaderResource, std::string const& property, glm::vec3 const& value)
{
	static_cast<G2DX11::ShaderResource*>(shaderResource)->setShaderUniformVec3(shaderResource, property, value);
}

void _setShaderUniformVec2Hlsl(G2Core::GfxResource* shaderResource, std::string const& property, glm::vec2 const& value)
{
	assert(false);
}


void _setShaderUniformVec2Cg(G2Core::GfxResource* shaderResource, std::string const& property, glm::vec2 const& value)
{
	G2DX11::CgShaderResource* cgRes = static_cast<G2DX11::CgShaderResource*>(shaderResource);
	auto location = cgRes->getAndCacheUniformLocation(property);
	if(location.first == nullptr) 
	{
		return;
	}
	cgSetParameter2fv(location.first, glm::value_ptr(value));
	cgRes->bindShader(shaderResource);
}

void SetShaderUniformVec2(G2Core::GfxResource* shaderResource, std::string const& property, glm::vec2 const& value)
{
	static_cast<G2DX11::ShaderResource*>(shaderResource)->setShaderUniformVec2(shaderResource, property, value);
}

void _setShaderUniformFloatHlsl(G2Core::GfxResource* shaderResource, std::string const& property, float value)
{
	assert(false);
}


void _setShaderUniformFloatCg(G2Core::GfxResource* shaderResource, std::string const& property, float value)
{
	G2DX11::CgShaderResource* cgRes = static_cast<G2DX11::CgShaderResource*>(shaderResource);
	auto location = cgRes->getAndCacheUniformLocation(property);
	if(location.first == nullptr) 
	{
		return;
	}
	cgSetParameter1f(location.first, value);
	cgRes->bindShader(shaderResource);
}

void SetShaderUniformFloat(G2Core::GfxResource* shaderResource, std::string const& property, float value)
{
	static_cast<G2DX11::ShaderResource*>(shaderResource)->setShaderUniformFloat(shaderResource, property, value);
}

void _setShaderUniformIntHlsl(G2Core::GfxResource* shaderResource, std::string const& property, int value)
{
	assert(false);
}

void _setShaderUniformIntCg(G2Core::GfxResource* shaderResource, std::string const& property, int value)
{
	G2DX11::CgShaderResource* cgRes = static_cast<G2DX11::CgShaderResource*>(shaderResource);
	auto location = cgRes->getAndCacheUniformLocation(property);
	if(location.first == nullptr) 
	{
		return;
	}
	cgSetParameter1i(location.first, value);
	cgRes->bindShader(shaderResource);
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
	case G2DX11::CG_SHADER:
		r->bindShader = _bindShaderCg;
		r->setShaderUniformMat4 = _setShaderUniformMat4Cg;
		r->setShaderUniformMat3 = _setShaderUniformMat3Cg;
		r->setShaderUniformVec4 = _setShaderUniformVec4Cg;
		r->setShaderUniformVec3 = _setShaderUniformVec3Cg;
		r->setShaderUniformVec2 = _setShaderUniformVec2Cg;
		r->setShaderUniformFloat = _setShaderUniformFloatCg;
		r->setShaderUniformInt = _setShaderUniformIntCg;
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