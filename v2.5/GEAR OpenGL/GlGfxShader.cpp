#include "GlGfxApi.h"
#include "GlGfxData.h"
#include "GlGfxMappings.h"

#include <G2/Logger.h>

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
	return error();
}

void _bindShaderGlsl(G2Core::GfxResource const* shaderResource)
{
	G2GL::GlslShaderResource const* glslRes = static_cast<G2GL::GlslShaderResource const*>(shaderResource);
	GLCHECK( glUseProgram(glslRes->programId) );
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


void SetShaderUniformMat4(G2Core::GfxResource* shaderResource, std::string const& property, glm::mat4 const& value)
{
	static_cast<G2GL::ShaderResource*>(shaderResource)->setShaderUniformMat4(shaderResource, property, value);
}

void _setShaderUniformMat3Glsl(G2Core::GfxResource* shaderResource, std::string const& property, glm::mat3 const& value)
{
	G2GL::GlslShaderResource* glslRes = static_cast<G2GL::GlslShaderResource*>(shaderResource);;
	GLCHECK( glUniformMatrix3fv(glslRes->getAndCacheUniformLocation(property), 1, GL_FALSE, glm::value_ptr(value)) );
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

void SetShaderUniformVec4(G2Core::GfxResource* shaderResource, std::string const& property, glm::vec4 const& value)
{
	static_cast<G2GL::ShaderResource*>(shaderResource)->setShaderUniformVec4(shaderResource, property, value);
}

void _setShaderUniformVec3Glsl(G2Core::GfxResource* shaderResource, std::string const& property, glm::vec3 const& value)
{
	G2GL::GlslShaderResource* glslRes = static_cast<G2GL::GlslShaderResource*>(shaderResource);;
	GLCHECK( glUniform3fv(glslRes->getAndCacheUniformLocation(property), 1, glm::value_ptr(value)) );
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

void SetShaderUniformVec2(G2Core::GfxResource* shaderResource, std::string const& property, glm::vec2 const& value)
{
	static_cast<G2GL::ShaderResource*>(shaderResource)->setShaderUniformVec2(shaderResource, property, value);
}

void _setShaderUniformFloatGlsl(G2Core::GfxResource* shaderResource, std::string const& property, float value)
{
	G2GL::GlslShaderResource* glslRes = static_cast<G2GL::GlslShaderResource*>(shaderResource);
	GLCHECK( glUniform1f(glslRes->getAndCacheUniformLocation(property), value) );
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

void SetShaderUniformInt(G2Core::GfxResource* shaderResource, std::string const& property, int value)
{
	static_cast<G2GL::ShaderResource*>(shaderResource)->setShaderUniformInt(shaderResource, property, value);
}

G2Core::GfxResource* _setupUniformBufferObjectFunctionPointers(G2Core::GfxResource* res);

G2Core::GfxResource* CreateUBO(std::string const& shadingLanguage, int size, void const* data, G2Core::BufferUsage::Name bufferUsage)
{
	if (shadingLanguage == "GLSL")
	{
		unsigned int uboId;
		GLCHECK(glGenBuffers(1, &uboId));
		GLCHECK(glBindBuffer(GL_UNIFORM_BUFFER, uboId));
		GLCHECK(glBufferData(GL_UNIFORM_BUFFER, size, data, toGlBufferUsage(bufferUsage)));

		// create client resource
		G2GL::GlslUniformBufferResource* resource = new G2GL::GlslUniformBufferResource(uboId);
		return _setupUniformBufferObjectFunctionPointers(resource);
	}
	return error();
}

void _bindUniformBufferGlsl(G2Core::GfxResource* ubo)
{
	G2GL::GlslUniformBufferResource* glslRes = static_cast<G2GL::GlslUniformBufferResource*>(ubo);
	GLCHECK(glBindBuffer(GL_UNIFORM_BUFFER, glslRes->uboId));
}

void BindUBO(G2Core::GfxResource* ubo)
{
	static_cast<G2GL::UniformBufferResource*>(ubo)->bindUniformBuffer(ubo);
}

void _unbindUniformBufferGlsl(G2Core::GfxResource* ubo)
{
	G2GL::GlslUniformBufferResource* glslRes = static_cast<G2GL::GlslUniformBufferResource*>(ubo);
	GLCHECK(glBindBuffer(GL_UNIFORM_BUFFER, 0));
}

void UnbindUBO(G2Core::GfxResource* ubo)
{
	static_cast<G2GL::UniformBufferResource*>(ubo)->unbindUniformBuffer(ubo);
}

void _setUBOBindingPointGlsl(G2Core::GfxResource* ubo, G2Core::UniformBufferBindingPoint::Name bindingPoint)
{
	G2GL::GlslUniformBufferResource* glslRes = static_cast<G2GL::GlslUniformBufferResource*>(ubo);
	glslRes->bindingPoint = (unsigned int)bindingPoint;
	GLCHECK(glBindBufferBase(GL_UNIFORM_BUFFER, glslRes->bindingPoint, glslRes->uboId));
}

void SetUBOBindingPoint(G2Core::GfxResource* ubo, G2Core::UniformBufferBindingPoint::Name bindingPoint)
{
	static_cast<G2GL::UniformBufferResource*>(ubo)->setUBOBindingPoint(ubo, bindingPoint);
}

void _setShaderUBOBlockBindingGlsl(G2Core::GfxResource* shaderResource, G2Core::GfxResource* ubo, std::string const& uboBlockName)
{
	G2GL::UniformBufferResource* uboRes = static_cast<G2GL::UniformBufferResource*>(ubo);
	G2GL::GlslShaderResource* glslRes = static_cast<G2GL::GlslShaderResource*>(shaderResource);

	GLCHECK(unsigned int blockIndex = glGetUniformBlockIndex(glslRes->programId, uboBlockName.c_str()));
	if (blockIndex != GL_INVALID_INDEX)
	{
		GLCHECK(glUniformBlockBinding(glslRes->programId, blockIndex, uboRes->bindingPoint));
	}
}

void SetShaderUBOBlockBinding(G2Core::GfxResource* shaderResource, G2Core::GfxResource* ubo, std::string const& uboBlockName)
{
	static_cast<G2GL::ShaderResource*>(shaderResource)->setShaderUBOBlockBinding(shaderResource, ubo, uboBlockName);
}

void _updateUBOSubDataGlsl(G2Core::GfxResource* ubo, unsigned int byteOffset, unsigned int byteSize, void* data)
{
	G2GL::GlslUniformBufferResource* glslRes = static_cast<G2GL::GlslUniformBufferResource*>(ubo);
	GLCHECK(glBufferSubData(GL_UNIFORM_BUFFER, byteOffset, byteSize, data));

}

void UpdateUBOSubData(G2Core::GfxResource* ubo, unsigned int byteOffset, unsigned int byteSize, void* data)
{
	static_cast<G2GL::UniformBufferResource*>(ubo)->updateUBOSubData(ubo, byteOffset, byteSize, data);
}

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
		r->setShaderUBOBlockBinding = _setShaderUBOBlockBindingGlsl;
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
		r->setShaderUBOBlockBinding = nullptr;
		break;
	}
	return res;
}

G2Core::GfxResource* _setupUniformBufferObjectFunctionPointers(G2Core::GfxResource* res)
{
	G2GL::UniformBufferResource* r = static_cast<G2GL::UniformBufferResource*>(res);
	switch (type(res))
	{
	case G2GL::GLSL_UBO:
		r->bindUniformBuffer = _bindUniformBufferGlsl;
		r->unbindUniformBuffer = _unbindUniformBufferGlsl;
		r->setUBOBindingPoint = _setUBOBindingPointGlsl;
		r->updateUBOSubData = _updateUBOSubDataGlsl;
		break;
	default:
		r->bindUniformBuffer = nullptr;
		r->unbindUniformBuffer = nullptr;
		break;
	}
	return res;
}