// GEAR 2.5 - Game Engine Andy Reimann - Author: Andy Reimann <andy@moorlands-grove.de>
// (c) 2014 GEAR 2.5
#include "GlslShader.h"
#include "Defines.h"
#include "Logger.h"

#include <GL/glew.h>
#include <glm/ext.hpp>

using namespace G2;

GlslShader::GlslShader() 
    : mVertexShaderId(0),
    mFragmentShaderId(0),
    mProgramId(0)
{
}

GlslShader::~GlslShader() 
{
    GLDEBUG( glDetachShader(mProgramId, mVertexShaderId) );
    GLDEBUG( glDetachShader(mProgramId, mFragmentShaderId) );

    GLDEBUG( glDeleteShader(mFragmentShaderId) );
    GLDEBUG( glDeleteShader(mVertexShaderId) );

    GLDEBUG( glDeleteProgram(mProgramId) );
}

void
GlslShader::bind() 
{
    // GL_INVALID_VALUE is generated if mProgramId is neither 0 nor a value generated by OpenGL.
    GLDEBUG( glUseProgram(mProgramId) );
}

void
GlslShader::setProperty(std::string const& property, glm::mat4 const& value) 
{
    GLDEBUG( glUniformMatrix4fv(getAndCacheUniformLocation(property), 1, GL_FALSE, glm::value_ptr(value)) );
}

void
GlslShader::setProperty(std::string const& property, glm::mat3 const& value) 
{
    GLDEBUG( glUniformMatrix3fv(getAndCacheUniformLocation(property), 1, GL_FALSE, glm::value_ptr(value)) );
}

void
GlslShader::setProperty(std::string const& property, glm::vec4 const& value) 
{
    GLDEBUG( glUniform4fv(getAndCacheUniformLocation(property), 1, glm::value_ptr(value)) );
}

void
GlslShader::setProperty(std::string const& property, glm::vec3 const& value) 
{
    GLDEBUG( glUniform3fv(getAndCacheUniformLocation(property), 1, glm::value_ptr(value)) );
}

void
GlslShader::setProperty(std::string const& property, float value) 
{
    GLDEBUG( glUniform1f(getAndCacheUniformLocation(property), value) );
}

void
GlslShader::setProperty(std::string const& property, int value) 
{
    GLDEBUG( glUniform1i(getAndCacheUniformLocation(property), value) );
}

bool
GlslShader::compile(std::string const& vertexCode, std::string const& fragmentCode) 
{
    mCompiled = false;
    const char* vertexShaderCode = vertexCode.c_str();
    const char* fragmentShaderCode = fragmentCode.c_str();
    GLDEBUG( mVertexShaderId = glCreateShader(GL_VERTEX_SHADER) );
    GLDEBUG( glShaderSource(mVertexShaderId, 1, &vertexShaderCode, NULL) );
    GLDEBUG( glCompileShader(mVertexShaderId) );
    std::string errorLog = getShaderInfoLog(mVertexShaderId);
    if(errorLog.length() > 0)
    {
        logger << "[GlslShader] Error: Compilation of Vertex Shader failed: \n" << errorLog;
        return mCompiled;
    }

    GLDEBUG( mFragmentShaderId = glCreateShader(GL_FRAGMENT_SHADER) );
    GLDEBUG( glShaderSource(mFragmentShaderId, 1, &fragmentShaderCode, NULL) );
    GLDEBUG( glCompileShader(mFragmentShaderId) );
    errorLog = getShaderInfoLog(mFragmentShaderId);
    if(errorLog.length() > 0) 
    {
        logger << "[GlslShader] Error: Compilation of Fragment Shader failed: \n" << errorLog;
        return mCompiled;
    }

    GLDEBUG( mProgramId = glCreateProgram() );
    GLDEBUG( glAttachShader(mProgramId, mVertexShaderId) );
    GLDEBUG( glAttachShader(mProgramId, mFragmentShaderId) );
    GLDEBUG( glLinkProgram(mProgramId) );
    errorLog = getProgramInfoLog(mProgramId);
    if(errorLog.length() > 0) 
    {
        logger << "[GlslShader] Error: Shader linker failed: \n" << errorLog;
        return mCompiled;
    }
    return mCompiled = true;
}

int
GlslShader::getAndCacheUniformLocation(std::string const& name) 
{
    auto it = mUniformLocations.find(name);
    if(it != mUniformLocations.end()) 
    {
        return it->second;
    }
    GLDEBUG( int location = glGetUniformLocation(mProgramId, name.c_str()) );
    // here we also cache not found uniforms to not endless search for them
    mUniformLocations[name] = location;
    return location;
}

std::string
GlslShader::getShaderInfoLog(int shader)
{
    GLint infologLength = 0;
    GLsizei charsWritten  = 0;
    GLchar* infoLog;

    GLDEBUG( glGetShaderiv(shader, GL_INFO_LOG_LENGTH,&infologLength) );

    if (infologLength > 1)
    {
        infoLog = (GLchar*)malloc(infologLength);
        GLDEBUG( glGetShaderInfoLog(shader, infologLength, &charsWritten, infoLog) );
        std::string ret(infoLog);
        free(infoLog);
        return ret;
    }
    return std::string("");
}

std::string
GlslShader::getProgramInfoLog(int program) 
{
    GLint infologLength = 0;
    GLsizei charsWritten  = 0;
    GLchar* infoLog;

    GLDEBUG( glGetProgramiv(program, GL_INFO_LOG_LENGTH,&infologLength) );

    if (infologLength > 1) 
    {
        infoLog = (GLchar*)malloc(infologLength);
        GLDEBUG( glGetProgramInfoLog(program, infologLength, &charsWritten, infoLog) );
        std::string ret(infoLog);
        free(infoLog);
        return ret;
    }
    return std::string("");
}