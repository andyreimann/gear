#pragma once
#include "DX11GfxApi.h"

#include <Cg/cgD3D11.h>

#include <G2Core/GfxDevice.h>

#include <unordered_map>
#include <vector>

namespace G2DX11
{
	enum Type 
	{
		INVALID_RESOURCE = 0,
		HLSL_SHADER,
		CG_SHADER,
	};
	struct DX11Resource : G2Core::GfxResource
	{
		DX11Resource(Type type) : type(type) {}
		Type type;
	};

	struct InputLayoutShaderFragment
	{
		// This struct holds all data for the final InputLayout 
		// Which is known at the shader compilation time
		std::vector<G2Core::Semantics::Name> semantics; // All semantics this shader requests
		std::vector<LPCSTR> semanticNames;			// All semantics this shader requests in the requested string format
	};

	typedef void (*BindShader)(G2Core::GfxResource const* shaderResource);
	typedef void (*SetShaderUniformMat4)(G2Core::GfxResource* shaderResource, std::string const& property, glm::mat4 const& value);
	typedef void (*SetShaderUniformMat3)(G2Core::GfxResource* shaderResource, std::string const& property, glm::mat3 const& value);
	typedef void (*SetShaderUniformVec4)(G2Core::GfxResource* shaderResource, std::string const& property, glm::vec4 const& value);
	typedef void (*SetShaderUniformVec3)(G2Core::GfxResource* shaderResource, std::string const& property, glm::vec3 const& value);
	typedef void (*SetShaderUniformVec2)(G2Core::GfxResource* shaderResource, std::string const& property, glm::vec2 const& value);
	typedef void (*SetShaderUniformFloat)(G2Core::GfxResource* shaderResource, std::string const& property, float value);
	typedef void (*SetShaderUniformInt)(G2Core::GfxResource* shaderResource, std::string const& property, int value);
	typedef void (*FreeGfxResource)(G2Core::GfxResource* resource);

	struct ShaderResource : DX11Resource
	{
		ShaderResource(Type type,ID3D10Blob* vertexShaderBlob, InputLayoutShaderFragment const& inputLayoutShaderFragment) 
			: DX11Resource(type),
			vertexShaderBlob(vertexShaderBlob),
			inputLayoutShaderFragment(inputLayoutShaderFragment) {}

		BindShader bindShader;
		SetShaderUniformMat4 setShaderUniformMat4;
		SetShaderUniformMat3 setShaderUniformMat3;
		SetShaderUniformVec4 setShaderUniformVec4;
		SetShaderUniformVec3 setShaderUniformVec3;
		SetShaderUniformVec2 setShaderUniformVec2;
		SetShaderUniformFloat setShaderUniformFloat;
		SetShaderUniformInt setShaderUniformInt;
		FreeGfxResource freeGfxResource;

		ID3D10Blob* vertexShaderBlob;
		InputLayoutShaderFragment inputLayoutShaderFragment;
	};

	struct CgShaderResource : ShaderResource
	{
		CgShaderResource(ID3D10Blob* vertexShaderBlob, InputLayoutShaderFragment const& inputLayoutShaderFragment)
			: vertexShaderId(nullptr),
			geometryShaderId(nullptr),
			fragmentShaderId(nullptr),
			ShaderResource(G2DX11::CG_SHADER,vertexShaderBlob, inputLayoutShaderFragment) {}
		~CgShaderResource()
		{
			cgDestroyProgram(vertexShaderId);
			if(geometryShaderId != nullptr)
			{
				cgDestroyProgram(geometryShaderId);
			}
			cgDestroyProgram(fragmentShaderId);
		}
		/** This function will try to cache and get the location of the 
		 * uniform variable for the given name.
		 * @param name The name of the uniform to get the location for.
		 * @return The location of the uniform or -1 if it could not be found.
		 */
		std::pair<CGparameter,CGprogram> const& getAndCacheUniformLocation(std::string const& name)
		{
			auto it = uniformCache.find(name);
			if(it != uniformCache.end()) 
			{
				return it->second;
			}

			CGparameter location = cgGetNamedParameter(vertexShaderId, name.c_str());
			if(location == nullptr) 
			{
				location = cgGetNamedParameter(fragmentShaderId, name.c_str());
				uniformCache[name] = std::make_pair(location,fragmentShaderId);
			}
			else 
			{
				uniformCache[name] = std::make_pair(location,vertexShaderId);
			}
			return uniformCache[name];
		}

		CGprogram vertexShaderId;
		CGprogram geometryShaderId;
		CGprogram fragmentShaderId;
		std::unordered_map<std::string,std::pair<CGparameter,CGprogram>> uniformCache;	// The cache for the uniform locations
	};

};