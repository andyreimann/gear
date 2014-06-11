// GEAR 2.5 - Game Engine Andy Reimann - Author: Andy Reimann <andy@moorlands-grove.de>
// (c) 2014 GEAR 2.5
#pragma once
#include "Shader.h"
#include "Defines.h"

#include <unordered_map>
#include <map>

#include <Cg/cgGL.h>

namespace G2 
{
	/** This class implements the generic Shader interface for the Cg shading language.
	 * It is designed to hold as few informations as possible to be really lightweight.
	 * @created:	2014/02/10
	 * @author Andy Reimann <a.reimann@moorlands-grove.de>
	 */
	class CgShader : public Shader
	{
		public:
			/** This constructs a new GlslShader.
			 */
			CgShader();
			/** This function will try to bind the given Shader.
			 * @note Make sure to only bind Shaders, which are bindable!
			 */
			virtual void bind();
			/** This function will set the given Property on the Shader to the given value.
			 * @param property The Property to set
			 * @param value The value to set the Property to.
			 */
			virtual void setProperty(std::string const& property, glm::mat4 const& value);
			/** This function will set the given Property on the Shader to the given value.
			 * @param property The Property to set
			 * @param value The value to set the Property to.
			 */
			virtual void setProperty(std::string const& property, glm::mat3 const& value);
			/** This function will set the given Property on the Shader to the given value.
			 * @param property The Property to set
			 * @param value The value to set the Property to.
			 */
			virtual void setProperty(std::string const& property, glm::vec4 const& value);
			/** This function will set the given Property on the Shader to the given value.
			 * @param property The Property to set
			 * @param value The value to set the Property to.
			 */
			virtual void setProperty(std::string const& property, glm::vec3 const& value);
			/** This function will set the given Property on the Shader to the given value.
			 * @param property The Property to set
			 * @param value The value to set the Property to.
			 */
			virtual void setProperty(std::string const& property, glm::vec2 const& value);
			/** This function will set the given Property on the Shader to the given value.
			 * @param property The Property to set
			 * @param value The value to set the Property to.
			 */
			virtual void setProperty(std::string const& property, float value);
			/** This function will set the given Property on the Shader to the given value.
			 * @param property The Property to set
			 * @param value The value to set the Property to.
			 */
			virtual void setProperty(std::string const& property, int value);
			/** normal destructor
			 */
			virtual ~CgShader();

			static void initContext();

		protected:
			
			/** This function will compile a new Shader from the given shader code.
			 * @param vertexCode The Vertex-Shader code to use
			 * @param geometryCode The Geometry-Shader code to use or an empty string if no geometry shader should be used.
			 * @param fragmentCode The Fragment-Shader code to use
			 * @return True if the Shader compiled successfully, false if not.
			 */
			virtual bool compile(std::string const& vertexCode, std::string const& geometryCode, std::string const& fragmentCode);
			/** This function will try to cache and get the location of the 
			 * uniform variable for the given name.
			 * @param name The name of the uniform to get the location for.
			 * @return The location of the uniform or -1 if it could not be found.
			 */
			std::pair<CGparameter,CGprogram> const& getAndCacheUniformLocation(std::string const& name);

			std::unordered_map<std::string,std::pair<CGparameter,CGprogram>> mUniformLocations;	// The cache for the uniform locations
			
			static CGcontext gCgContext;

			static CGprofile	gCgVertexShaderProfile;	// Vertex Shader Profile
			static CGprofile	gCgGeometryShaderProfile;	// Geometry Shader Profile
			static CGprofile	gCgFragmentShaderProfile;	// Fragment Shader Profile


			CGprogram	mVertexShaderId;	// The vertex shader id
			CGprogram	mGeometryShaderId;	// The geometry shader id	
			CGprogram	mFragmentShaderId;	// The fragment shader id	
	};
};