// GEAR 2.5 - Game Engine Andy Reimann - Author: Andy Reimann <andy@moorlands-grove.de>
// (c) 2014 GEAR 2.5
#include "CgShader.h"
#include "Defines.h"
#include "Logger.h"

#include <GL/glew.h>
#include <glm/ext.hpp>

using namespace G2;

CgShader::CgShader() 
	: mVertexShaderId(nullptr),
	mGeometryShaderId(nullptr),
	mFragmentShaderId(nullptr)
{
	initContext();
}

CgShader::~CgShader() 
{
	cgDestroyProgram(mVertexShaderId);
	if(mGeometryShaderId != nullptr)
	{
		cgDestroyProgram(mGeometryShaderId);
	}
	cgDestroyProgram(mFragmentShaderId);
}

void
CgShader::bind() 
{
	GLDEBUG( glUseProgram(0) );
	cgGLEnableProfile(gCgVertexShaderProfile);
	cgGLEnableProfile(gCgFragmentShaderProfile);
	if(mGeometryShaderId != nullptr)
	{
		cgGLEnableProfile(gCgGeometryShaderProfile);
		cgGLBindProgram(mGeometryShaderId);
	}

	cgGLBindProgram(mVertexShaderId);
	cgGLBindProgram(mFragmentShaderId);
}

void
CgShader::setProperty(std::string const& property, glm::mat4 const& value) 
{
	auto location = getAndCacheUniformLocation(property);
	if(location.first == nullptr) 
	{
		return;
	}
	cgGLSetMatrixParameterfc(location.first, glm::value_ptr(value));
}

void
CgShader::setProperty(std::string const& property, glm::mat3 const& value) 
{
	auto location = getAndCacheUniformLocation(property);
	if(location.first == nullptr) 
	{
		return;
	}
	cgGLSetMatrixParameterfc(location.first, glm::value_ptr(value));
}

void
CgShader::setProperty(std::string const& property, glm::vec4 const& value) 
{
	auto location = getAndCacheUniformLocation(property);
	if(location.first == nullptr) 
	{
		return;
	}
	cgSetParameter4fv(location.first, glm::value_ptr(value));
}

void
CgShader::setProperty(std::string const& property, glm::vec3 const& value) 
{
	auto location = getAndCacheUniformLocation(property);
	if(location.first == nullptr) 
	{
		return;
	}
	cgSetParameter3fv(location.first, glm::value_ptr(value));
}

void
CgShader::setProperty(std::string const& property, glm::vec2 const& value) 
{
	auto location = getAndCacheUniformLocation(property);
	if(location.first == nullptr) 
	{
		return;
	}
	cgSetParameter2fv(location.first, glm::value_ptr(value));
}

void
CgShader::setProperty(std::string const& property, float value) 
{
	auto location = getAndCacheUniformLocation(property);
	if(location.first == nullptr) 
	{
		return;
	}
	cgSetParameter1f(location.first, value);
}

void
CgShader::setProperty(std::string const& property, int value) 
{
	auto location = getAndCacheUniformLocation(property);
	if(location.first == nullptr) 
	{
		return;
	}
	cgSetParameter1i(location.first, value);
}

bool
CgShader::compile(std::string const& vertexCode, std::string const& geometryCode, std::string const& fragmentCode) 
{
	mCompiled = false;
	if(gCgGeometryShaderProfile == CG_PROFILE_UNKNOWN && geometryCode != "")
	{
		return false; // error -> geometry shader given but not supported!
	}
	
	const char* sourcePtr = vertexCode.c_str();
	
	mVertexShaderId = cgCreateProgram(gCgContext, CG_SOURCE, sourcePtr, gCgVertexShaderProfile, "main", 0);
	
	if(geometryCode != "")
	{
		sourcePtr = geometryCode.c_str();
		mGeometryShaderId = cgCreateProgram(gCgContext, CG_SOURCE, sourcePtr, gCgGeometryShaderProfile, "main", 0);
	}
	sourcePtr = fragmentCode.c_str();
	mFragmentShaderId = cgCreateProgram(gCgContext, CG_SOURCE, sourcePtr, gCgFragmentShaderProfile, "main", 0);
	
	cgGLLoadProgram(mVertexShaderId);
	cgGLLoadProgram(mFragmentShaderId);
	

	return mCompiled = true;
}

std::pair<CGparameter,CGprogram> const&
CgShader::getAndCacheUniformLocation(std::string const& name) 
{
	auto it = mUniformLocations.find(name);
	if(it != mUniformLocations.end()) 
	{
		return it->second;
	}

	CGparameter location = cgGetNamedParameter(mVertexShaderId, name.c_str());
	if(location == nullptr) 
	{
		location = cgGetNamedParameter(mFragmentShaderId, name.c_str());
		mUniformLocations[name] = std::make_pair(location,mFragmentShaderId);
	}
	else 
	{
		mUniformLocations[name] = std::make_pair(location,mVertexShaderId);
	}
	return mUniformLocations[name];
}

void cgErrorHandler(CGcontext context, CGerror error, void* appData) 
{
	if(error != CG_NO_ERROR)
	{
		const char* strPtr = cgGetErrorString(error);
		logger << "[CgShader] Cg Error: \n" << std::string(strPtr != nullptr ? strPtr : "");
		
		if(error == CG_COMPILER_ERROR)
		{
			strPtr = cgGetLastListing(context);
			logger << std::string(strPtr != nullptr ? strPtr : "");
		}
		logger << endl;
	}
}

CGcontext		CgShader::gCgContext = nullptr;
CGprofile		CgShader::gCgVertexShaderProfile;
CGprofile		CgShader::gCgGeometryShaderProfile;
CGprofile		CgShader::gCgFragmentShaderProfile;

void
CgShader::initContext() 
{

	if(gCgContext == nullptr) {
		logger << "[Cg] Initialize Cg" << endl;
		//qDebug("[Cg] Initialize Cg");
		// register the error handler
		cgSetErrorHandler( &cgErrorHandler, NULL);
		// create a new Cg Context
		gCgContext = cgCreateContext();

		// Register the default state assignment for OpenGL
		cgGLRegisterStates(gCgContext);
		// This will allow the Cg runtime to manage texture binding
		cgGLSetManageTextureParameters(gCgContext, CG_TRUE);
		
		gCgVertexShaderProfile = cgGLGetLatestProfile(CG_GL_VERTEX);
		if(gCgVertexShaderProfile == CG_PROFILE_UNKNOWN)
		{
			// ERROR
			logger << "[Cg] Error: Could not get valid Vertex-Profile." << endl;
			return;
		}
		
		gCgGeometryShaderProfile = cgGLGetLatestProfile(CG_GL_GEOMETRY);
		if(gCgGeometryShaderProfile == CG_PROFILE_UNKNOWN)
		{
			// WARNING
			logger << "[Cg] Warning: Could not get valid Geometry-Profile." << endl;
		}
		
		gCgFragmentShaderProfile = cgGLGetLatestProfile(CG_GL_FRAGMENT);
		if(gCgFragmentShaderProfile == CG_PROFILE_UNKNOWN)
		{
			// ERROR
			logger << "[Cg] Error: Could not get valid Fragment-Profile." << endl;
			return;
		}
		cgGLSetOptimalOptions(gCgVertexShaderProfile);	
		if(gCgGeometryShaderProfile == CG_PROFILE_UNKNOWN)
		{
			cgGLSetOptimalOptions(gCgGeometryShaderProfile);	
		}
		cgGLSetOptimalOptions(gCgFragmentShaderProfile);
	}
}