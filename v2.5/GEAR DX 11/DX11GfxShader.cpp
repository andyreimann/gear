#include "DX11GfxApi.h"
#include "DX11GfxData.h"
#include "DX11GfxMappings.h"

#include <G2/Logger.h>

#include <windowsx.h> 
#include <d3d11.h>		// core
#include <d3dx11.h>		// extensions
#include <d3dx10.h>		// dx 11 is ext of dx 10 -> useful macros usable

CGcontext gCgContext = nullptr;
CGprofile gCgVertexShaderProfile;
CGprofile gCgGeometryShaderProfile;
CGprofile gCgFragmentShaderProfile;

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

		// Register the default state assignment for OpenGL
		cgD3D11RegisterStates(gCgContext);
		// This will allow the Cg runtime to manage texture binding
		cgD3D11SetManageTextureParameters(gCgContext, CG_TRUE);
		
		gCgVertexShaderProfile = cgD3D11GetLatestVertexProfile();
		if(gCgVertexShaderProfile == CG_PROFILE_UNKNOWN)
		{
			// ERROR
			G2::logger << "[CgRuntime] Error: Could not get valid Vertex-Profile." << G2::endl;
			return;
		}
		
		gCgGeometryShaderProfile = cgD3D11GetLatestGeometryProfile();
		if(gCgGeometryShaderProfile == CG_PROFILE_UNKNOWN)
		{
			// WARNING
			G2::logger << "[CgRuntime] Warning: Could not get valid Geometry-Profile." << G2::endl;
		}
		
		gCgFragmentShaderProfile = cgD3D11GetLatestPixelProfile();
		if(gCgFragmentShaderProfile == CG_PROFILE_UNKNOWN)
		{
			// ERROR
			G2::logger << "[CgRuntime] Error: Could not get valid Fragment-Profile." << G2::endl;
			return;
		}
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

		if(gCgGeometryShaderProfile == CG_PROFILE_UNKNOWN && geometryCode != "")
		{
			return error(); // geometry shader given but not supported!
		}
	
		const char* sourcePtr = vertexCode.c_str();
	
		int D3DCOMPILE_OPTIMIZATION_LEVEL3 = (1 << 15);
		// http://msdn.microsoft.com/en-us/library/windows/desktop/gg615083(v=vs.85).aspx

		CGprogram vertexShaderId = cgCreateProgram(gCgContext, CG_SOURCE, sourcePtr, gCgVertexShaderProfile, "main", 0);
		CGprogram geometryShaderId = nullptr;
		if(geometryCode != "")
		{
			sourcePtr = geometryCode.c_str();
			geometryShaderId = cgCreateProgram(gCgContext, CG_SOURCE, sourcePtr, gCgGeometryShaderProfile, "main", 0);
			
			cgD3D11LoadProgram(geometryShaderId, D3DCOMPILE_OPTIMIZATION_LEVEL3);
		}
		sourcePtr = fragmentCode.c_str();
		
		/*std::vector<D3D11_INPUT_ELEMENT_DESC> layout;
		
		for(int i = 0; i < vertexInputLayout.elements.size(); ++i)
		{
			D3D11_INPUT_ELEMENT_DESC element;
			element.SemanticName = toSemanticString(vertexInputLayout.elements[i].semantic);
			element.SemanticIndex = 0;
			element.Format = DXGI_FORMAT_R32G32B32_FLOAT;
			element.InputSlot = i;
			element.AlignedByteOffset = 0;
			element.InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
			element.InstanceDataStepRate = 0;

			layout.push_back(std::move(element));
		}*/

		CGprogram fragmentShaderId = cgCreateProgram(gCgContext, CG_SOURCE, sourcePtr, gCgFragmentShaderProfile, "main", 0);

		cgD3D11LoadProgram(vertexShaderId, D3DCOMPILE_OPTIMIZATION_LEVEL3);
		cgD3D11LoadProgram(fragmentShaderId, D3DCOMPILE_OPTIMIZATION_LEVEL3);

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
			
	if(cgRes->geometryShaderId != nullptr)
	{
		cgD3D11BindProgram(cgRes->geometryShaderId);
	}
	cgD3D11BindProgram(cgRes->vertexShaderId);
	cgD3D11BindProgram(cgRes->fragmentShaderId);
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