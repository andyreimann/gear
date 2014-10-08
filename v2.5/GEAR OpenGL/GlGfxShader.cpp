#include "GlGfxApi.h"
#include "GlGfxData.h"

#include <G2/Logger.h>

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

void _initCgRuntime()
{
	if(gCgContext == nullptr) {
		G2::logger << "[CgRuntime] Initialize Cg-Runtime-OpenGL" << G2::endl;
		//qDebug("[Cg] Initialize Cg");
		// register the error handler
		cgSetErrorHandler( &_cgErrorHandler, NULL);
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
			G2::logger << "[CgRuntime] Error: Could not get valid Vertex-Profile." << G2::endl;
			return;
		}
		
		gCgGeometryShaderProfile = cgGLGetLatestProfile(CG_GL_GEOMETRY);
		if(gCgGeometryShaderProfile == CG_PROFILE_UNKNOWN)
		{
			// WARNING
			G2::logger << "[CgRuntime] Warning: Could not get valid Geometry-Profile." << G2::endl;
		}
		
		gCgFragmentShaderProfile = cgGLGetLatestProfile(CG_GL_FRAGMENT);
		if(gCgFragmentShaderProfile == CG_PROFILE_UNKNOWN)
		{
			// ERROR
			G2::logger << "[CgRuntime] Error: Could not get valid Fragment-Profile." << G2::endl;
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

std::string
_glslGetShaderInfoLog(int shader)
{
	GLint infologLength = 0;
	GLsizei charsWritten  = 0;
	GLchar* infoLog;

	GLCHECK( glGetShaderiv(shader, GL_INFO_LOG_LENGTH,&infologLength) );

	if (infologLength > 1)
	{
		infoLog = (GLchar*)malloc(infologLength);
		GLCHECK( glGetShaderInfoLog(shader, infologLength, &charsWritten, infoLog) );
		std::string ret(infoLog);
		free(infoLog);
		return ret;
	}
	return std::string("");
}

std::string
_glslGetProgramInfoLog(int program) 
{
	GLint infologLength = 0;
	GLsizei charsWritten  = 0;
	GLchar* infoLog;

	GLCHECK(glGetProgramiv(program, GL_INFO_LOG_LENGTH, &infologLength));

	if (infologLength > 1) 
	{
		infoLog = (GLchar*)malloc(infologLength);
		GLCHECK( glGetProgramInfoLog(program, infologLength, &charsWritten, infoLog) );
		std::string ret(infoLog);
		free(infoLog);
		return ret;
	}
	return std::string("");
}

G2Core::GfxResource* _setupShaderFunctionPointers(G2Core::GfxResource* res);

G2Core::GfxResource* CompileShader(G2Core::VertexInputLayout const& vertexInputLayout, std::string const& shadingLanguage, std::string const& vertexCode, std::string const& geometryCode, std::string const& fragmentCode)
{
	if(shadingLanguage == "GLSL")
	{
		const char* vertexShaderCode = vertexCode.c_str();
		GLCHECK( int vertexShaderId = glCreateShader(GL_VERTEX_SHADER) );
		GLCHECK( glShaderSource(vertexShaderId, 1, &vertexShaderCode, NULL) );
		GLCHECK( glCompileShader(vertexShaderId) );
		std::string errorLog = _glslGetShaderInfoLog(vertexShaderId);
		if(errorLog.length() > 0)
		{
			G2::logger << "[GlslShader] Error: Compilation of Vertex Shader failed: \n" << errorLog;
			return error();
		}
		int geometryShaderId = 0;
		if(geometryCode != "")
		{
			const char* geometryShaderCode = geometryCode.c_str();
			GLCHECK( geometryShaderId = glCreateShader(GL_GEOMETRY_SHADER) );
			GLCHECK( glShaderSource(geometryShaderId, 1, &geometryShaderCode, NULL) );
			GLCHECK( glCompileShader(geometryShaderId) );
			std::string errorLog = _glslGetShaderInfoLog(geometryShaderId);
			if(errorLog.length() > 0)
			{
				G2::logger << "[GlslShader] Error: Compilation of Geometry Shader failed: \n" << errorLog;
				return error();
			}
		}
	
		const char* fragmentShaderCode = fragmentCode.c_str();
		GLCHECK( int fragmentShaderId = glCreateShader(GL_FRAGMENT_SHADER) );
		GLCHECK( glShaderSource(fragmentShaderId, 1, &fragmentShaderCode, NULL) );
		GLCHECK( glCompileShader(fragmentShaderId) );
		errorLog = _glslGetShaderInfoLog(fragmentShaderId);
		if(errorLog.length() > 0) 
		{
			G2::logger << "[GlslShader] Error: Compilation of Fragment Shader failed: \n" << errorLog;
			return error();
		}

		GLCHECK( int programId = glCreateProgram() );
		GLCHECK( glAttachShader(programId, vertexShaderId) );
		if(geometryCode != "")
		{
			GLCHECK( glAttachShader(programId, geometryShaderId) );
		}
		GLCHECK( glAttachShader(programId, fragmentShaderId) );
		GLCHECK( glLinkProgram(programId) );
		errorLog = _glslGetProgramInfoLog(programId);
		if(errorLog.length() > 0) 
		{
			G2::logger << "[GlslShader] Error: Shader linker failed: \n" << errorLog;
			return error();
		}
		// create client resource
		G2GL::GlslShaderResource* resource = new G2GL::GlslShaderResource;
		resource->vertexShaderId = vertexShaderId;
		resource->geometryShaderId = geometryShaderId;
		resource->fragmentShaderId = fragmentShaderId;
		resource->programId = programId;
		return _setupShaderFunctionPointers(resource);
	}
	else if(shadingLanguage == "CG")
	{

		if(gCgGeometryShaderProfile == CG_PROFILE_UNKNOWN && geometryCode != "")
		{
			return error(); // geometry shader given but not supported!
		}
	
		const char* sourcePtr = vertexCode.c_str();
	
		CGprogram vertexShaderId = cgCreateProgram(gCgContext, CG_SOURCE, sourcePtr, gCgVertexShaderProfile, "main", 0);
		CGprogram geometryShaderId = nullptr;
		if(geometryCode != "")
		{
			sourcePtr = geometryCode.c_str();
			geometryShaderId = cgCreateProgram(gCgContext, CG_SOURCE, sourcePtr, gCgGeometryShaderProfile, "main", 0);
			cgGLLoadProgram(geometryShaderId);
		}
		sourcePtr = fragmentCode.c_str();
		CGprogram fragmentShaderId = cgCreateProgram(gCgContext, CG_SOURCE, sourcePtr, gCgFragmentShaderProfile, "main", 0);
	
		cgGLLoadProgram(vertexShaderId);
		cgGLLoadProgram(fragmentShaderId);

		// create client resource
		G2GL::CgShaderResource* resource = new G2GL::CgShaderResource;
		resource->vertexShaderId = vertexShaderId;
		resource->geometryShaderId = geometryShaderId;
		resource->fragmentShaderId = fragmentShaderId;
		return _setupShaderFunctionPointers(resource);
	}
	return error();
}

void _bindShaderGlsl(G2Core::GfxResource const* shaderResource)
{
	G2GL::GlslShaderResource const* glslRes = static_cast<G2GL::GlslShaderResource const*>(shaderResource);
	GLCHECK( glUseProgram(glslRes->programId) );
}

void _bindShaderCg(G2Core::GfxResource const* shaderResource)
{
	G2GL::CgShaderResource const* cgRes = static_cast<G2GL::CgShaderResource const*>(shaderResource);
			
	GLCHECK( glUseProgram(0) );
	cgGLEnableProfile(gCgVertexShaderProfile);
	cgGLEnableProfile(gCgFragmentShaderProfile);
	if(cgRes->geometryShaderId != nullptr)
	{
		cgGLEnableProfile(gCgGeometryShaderProfile);
		cgGLBindProgram(cgRes->geometryShaderId);
	}

	cgGLBindProgram(cgRes->vertexShaderId);
	cgGLBindProgram(cgRes->fragmentShaderId);
}

void BindShader(G2Core::GfxResource const* shaderResource)
{
	G2GL::ShaderResource const* res = static_cast<G2GL::ShaderResource const*>(shaderResource);
	res->bindShader(shaderResource);
}

void _setShaderUniformMat4Glsl(G2Core::GfxResource* shaderResource, std::string const& property, glm::mat4 const& value)
{
	G2GL::GlslShaderResource* glslRes = static_cast<G2GL::GlslShaderResource*>(shaderResource);;
	GLCHECK( glUniformMatrix4fv(glslRes->getAndCacheUniformLocation(property), 1, GL_FALSE, glm::value_ptr(value)) );
}

void _setShaderUniformMat4Cg(G2Core::GfxResource* shaderResource, std::string const& property, glm::mat4 const& value)
{
	G2GL::CgShaderResource* cgRes = static_cast<G2GL::CgShaderResource*>(shaderResource);
	auto location = cgRes->getAndCacheUniformLocation(property);
	if(location.first == nullptr) 
	{
		return;
	}
	cgGLSetMatrixParameterfc(location.first, glm::value_ptr(value));
}


void SetShaderUniformMat4(G2Core::GfxResource* shaderResource, std::string const& property, glm::mat4 const& value)
{
	static_cast<G2GL::ShaderResource*>(shaderResource)->setShaderUniformMat4(shaderResource, property, value);
}

void _setShaderUniformMat3Glsl(G2Core::GfxResource* shaderResource, std::string const& property, glm::mat3 const& value)
{
	G2GL::GlslShaderResource* glslRes = static_cast<G2GL::GlslShaderResource*>(shaderResource);;
	GLCHECK( glUniformMatrix3fv(glslRes->getAndCacheUniformLocation(property), 1, GL_FALSE, glm::value_ptr(value)) );
}

void _setShaderUniformMat3Cg(G2Core::GfxResource* shaderResource, std::string const& property, glm::mat3 const& value)
{
	G2GL::CgShaderResource* cgRes = static_cast<G2GL::CgShaderResource*>(shaderResource);
	auto location = cgRes->getAndCacheUniformLocation(property);
	if(location.first == nullptr) 
	{
		return;
	}
	cgGLSetMatrixParameterfc(location.first, glm::value_ptr(value));
}

void SetShaderUniformMat3(G2Core::GfxResource* shaderResource, std::string const& property, glm::mat3 const& value)
{
	static_cast<G2GL::ShaderResource*>(shaderResource)->setShaderUniformMat3(shaderResource, property, value);
}

void _setShaderUniformVec4Glsl(G2Core::GfxResource* shaderResource, std::string const& property, glm::vec4 const& value)
{
	G2GL::GlslShaderResource* glslRes = static_cast<G2GL::GlslShaderResource*>(shaderResource);;
	GLCHECK( glUniform4fv(glslRes->getAndCacheUniformLocation(property), 1, glm::value_ptr(value)) );
}

void _setShaderUniformVec4Cg(G2Core::GfxResource* shaderResource, std::string const& property, glm::vec4 const& value)
{
	G2GL::CgShaderResource* cgRes = static_cast<G2GL::CgShaderResource*>(shaderResource);
	auto location = cgRes->getAndCacheUniformLocation(property);
	if(location.first == nullptr) 
	{
		return;
	}
	cgSetParameter4fv(location.first, glm::value_ptr(value));
}

void SetShaderUniformVec4(G2Core::GfxResource* shaderResource, std::string const& property, glm::vec4 const& value)
{
	static_cast<G2GL::ShaderResource*>(shaderResource)->setShaderUniformVec4(shaderResource, property, value);
}

void _setShaderUniformVec3Glsl(G2Core::GfxResource* shaderResource, std::string const& property, glm::vec3 const& value)
{
	G2GL::GlslShaderResource* glslRes = static_cast<G2GL::GlslShaderResource*>(shaderResource);;
	GLCHECK( glUniform3fv(glslRes->getAndCacheUniformLocation(property), 1, glm::value_ptr(value)) );
}

void _setShaderUniformVec3Cg(G2Core::GfxResource* shaderResource, std::string const& property, glm::vec3 const& value)
{
	G2GL::CgShaderResource* cgRes = static_cast<G2GL::CgShaderResource*>(shaderResource);
	auto location = cgRes->getAndCacheUniformLocation(property);
	if(location.first == nullptr) 
	{
		return;
	}
	cgSetParameter3fv(location.first, glm::value_ptr(value));
}

void SetShaderUniformVec3(G2Core::GfxResource* shaderResource, std::string const& property, glm::vec3 const& value)
{
	static_cast<G2GL::ShaderResource*>(shaderResource)->setShaderUniformVec3(shaderResource, property, value);
}

void _setShaderUniformVec2Glsl(G2Core::GfxResource* shaderResource, std::string const& property, glm::vec2 const& value)
{
	G2GL::GlslShaderResource* glslRes = static_cast<G2GL::GlslShaderResource*>(shaderResource);;
	GLCHECK( glUniform2fv(glslRes->getAndCacheUniformLocation(property), 1, glm::value_ptr(value)) );
}

void _setShaderUniformVec2Cg(G2Core::GfxResource* shaderResource, std::string const& property, glm::vec2 const& value)
{
	G2GL::CgShaderResource* cgRes = static_cast<G2GL::CgShaderResource*>(shaderResource);
	auto location = cgRes->getAndCacheUniformLocation(property);
	if(location.first == nullptr) 
	{
		return;
	}
	cgSetParameter2fv(location.first, glm::value_ptr(value));
}

void SetShaderUniformVec2(G2Core::GfxResource* shaderResource, std::string const& property, glm::vec2 const& value)
{
	static_cast<G2GL::ShaderResource*>(shaderResource)->setShaderUniformVec2(shaderResource, property, value);
}

void _setShaderUniformFloatGlsl(G2Core::GfxResource* shaderResource, std::string const& property, float value)
{
	G2GL::GlslShaderResource* glslRes = static_cast<G2GL::GlslShaderResource*>(shaderResource);
	GLCHECK( glUniform1f(glslRes->getAndCacheUniformLocation(property), value) );
}

void _setShaderUniformFloatCg(G2Core::GfxResource* shaderResource, std::string const& property, float value)
{
	G2GL::CgShaderResource* cgRes = static_cast<G2GL::CgShaderResource*>(shaderResource);
	auto location = cgRes->getAndCacheUniformLocation(property);
	if(location.first == nullptr) 
	{
		return;
	}
	cgSetParameter1f(location.first, value);
}

void SetShaderUniformFloat(G2Core::GfxResource* shaderResource, std::string const& property, float value)
{
	static_cast<G2GL::ShaderResource*>(shaderResource)->setShaderUniformFloat(shaderResource, property, value);
}

void _setShaderUniformIntGlsl(G2Core::GfxResource* shaderResource, std::string const& property, int value)
{
	G2GL::GlslShaderResource* glslRes = static_cast<G2GL::GlslShaderResource*>(shaderResource);
	GLCHECK( glUniform1i(glslRes->getAndCacheUniformLocation(property), value) );
}

void _setShaderUniformIntCg(G2Core::GfxResource* shaderResource, std::string const& property, int value)
{
	G2GL::CgShaderResource* cgRes = static_cast<G2GL::CgShaderResource*>(shaderResource);
	auto location = cgRes->getAndCacheUniformLocation(property);
	if(location.first == nullptr) 
	{
		return;
	}
	cgSetParameter1i(location.first, value);
}

void SetShaderUniformInt(G2Core::GfxResource* shaderResource, std::string const& property, int value)
{
	static_cast<G2GL::ShaderResource*>(shaderResource)->setShaderUniformInt(shaderResource, property, value);
}

//void _destroyShaderGlsl(G2Core::GfxResource& shaderResource)
//{
//	G2GL::GlslShaderResource* glslRes = static_cast<G2GL::GlslShaderResource*>(&shaderResource);
//	GLCHECK( glDetachShader(glslRes->programId, glslRes->vertexShaderId) );
//	GLCHECK( glDetachShader(glslRes->programId, glslRes->geometryShaderId) );
//	GLCHECK( glDetachShader(glslRes->programId, glslRes->fragmentShaderId) );
//	
//	GLCHECK( glDeleteShader(glslRes->fragmentShaderId) );
//	GLCHECK( glDeleteShader(glslRes->geometryShaderId) );
//	GLCHECK( glDeleteShader(glslRes->vertexShaderId) );
//
//	GLCHECK( glDeleteProgram(glslRes->programId) );
//}
//
//void _destroyShaderCg(G2Core::GfxResource& shaderResource)
//{
//	G2GL::CgShaderResource* cgRes = static_cast<G2GL::CgShaderResource*>(&shaderResource);
//	cgDestroyProgram(cgRes->vertexShaderId);
//	if(cgRes->geometryShaderId != nullptr)
//	{
//		cgDestroyProgram(cgRes->geometryShaderId);
//	}
//	cgDestroyProgram(cgRes->fragmentShaderId);
//}

G2Core::GfxResource* _setupShaderFunctionPointers(G2Core::GfxResource* res)
{
	G2GL::ShaderResource* r = static_cast<G2GL::ShaderResource*>(res);
	switch(type(res))
	{
	case G2GL::GLSL_SHADER:
		r->bindShader = _bindShaderGlsl;
		r->setShaderUniformMat4 = _setShaderUniformMat4Glsl;
		r->setShaderUniformMat3 = _setShaderUniformMat3Glsl;
		r->setShaderUniformVec4 = _setShaderUniformVec4Glsl;
		r->setShaderUniformVec3 = _setShaderUniformVec3Glsl;
		r->setShaderUniformVec2 = _setShaderUniformVec2Glsl;
		r->setShaderUniformFloat = _setShaderUniformFloatGlsl;
		r->setShaderUniformInt = _setShaderUniformIntGlsl;
		break;
	case G2GL::CG_SHADER:
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