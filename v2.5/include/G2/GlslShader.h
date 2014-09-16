//// GEAR 2.5 - Game Engine Andy Reimann - Author: Andy Reimann <andy@moorlands-grove.de>
//// (c) 2014 GEAR 2.5
//#pragma once
//#include "Shader.h"
//
//#include <unordered_map>
//
//namespace G2 
//{
//	/** This class implements the generic Shader interface for the GLSL shading language.
//	 * It is designed to hold as few informations as possible to be really lightweight.
//	 * @created:	2014/02/10
//	 * @author Andy Reimann <a.reimann@moorlands-grove.de>
//	 */
//	class GlslShader : public Shader
//	{
//		public:
//			/** This constructs a new GlslShader.
//			 */
//			GlslShader();
//			/** This function will try to bind the given Shader.
//			 * @note Make sure to only bind Shaders, which are bindable!
//			 */
//			virtual void bind();
//			/** This function will set the given Property on the Shader to the given value.
//			 * @param property The Property to set
//			 * @param value The value to set the Property to.
//			 */
//			virtual void setProperty(std::string const& property, glm::mat4 const& value);
//			/** This function will set the given Property on the Shader to the given value.
//			 * @param property The Property to set
//			 * @param value The value to set the Property to.
//			 */
//			virtual void setProperty(std::string const& property, glm::mat3 const& value);
//			/** This function will set the given Property on the Shader to the given value.
//			 * @param property The Property to set
//			 * @param value The value to set the Property to.
//			 */
//			virtual void setProperty(std::string const& property, glm::vec4 const& value);
//			/** This function will set the given Property on the Shader to the given value.
//			 * @param property The Property to set
//			 * @param value The value to set the Property to.
//			 */
//			virtual void setProperty(std::string const& property, glm::vec3 const& value);
//			/** This function will set the given Property on the Shader to the given value.
//			 * @param property The Property to set
//			 * @param value The value to set the Property to.
//			 */
//			virtual void setProperty(std::string const& property, glm::vec2 const& value);
//			/** This function will set the given Property on the Shader to the given value.
//			 * @param property The Property to set
//			 * @param value The value to set the Property to.
//			 */
//			virtual void setProperty(std::string const& property, float value);
//			/** This function will set the given Property on the Shader to the given value.
//			 * @param property The Property to set
//			 * @param value The value to set the Property to.
//			 */
//			virtual void setProperty(std::string const& property, int value);
//			/** normal destructor
//			 */
//			virtual ~GlslShader();
//		protected:
//			/** This function will compile a new Shader from the given shader code.
//			 * @param vertexCode The Vertex-Shader code to use
//			 * @param geometryCode The Geometry-Shader code to use or an empty string if no geometry shader should be used.
//			 * @param fragmentCode The Fragment-Shader code to use
//			 * @return True if the Shader compiled successfully, false if not.
//			 */
//			virtual bool compile(std::string const& vertexCode, std::string const& geometryCode, std::string const& fragmentCode);
//			/** This function will try to cache and get the location of the 
//			 * uniform variable for the given name.
//			 * @param name The name of the uniform to get the location for.
//			 * @return The location of the uniform or -1 if it could not be found.
//			 */
//			int getAndCacheUniformLocation(std::string const& name);
//			/** This function will return an info log for the given shader id.
//			 * @param shader The shader id to get an info log for.
//			 * @return The shader info log, if some is available, an empty string else.
//			 */
//			static std::string getShaderInfoLog(int shader);
//			/** This function will return an info log for the given program id.
//			 * @param program The program id to get an info log for.
//			 * @return The program info log, if some is available, an empty string else.
//			 */
//			static std::string getProgramInfoLog(int program);
//
//			std::unordered_map<std::string,int>		mUniformLocations;	// The cache for the uniform locations
//
//			int			mVertexShaderId;	// The vertex shader id
//			int			mGeometryShaderId;	// The geometry shader id
//			int			mFragmentShaderId;	// The fragment shader id
//			int			mProgramId;			// The program id
//	};
//};