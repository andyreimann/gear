// GEAR 2.5 - Game Engine Andy Reimann - Author: Andy Reimann <andy@moorlands-grove.de>
// (c) 2014 GEAR 2.5
#pragma once
#include "Property.h"
#include "MacroCondition.h"
#include "ShaderMetaData.h"
#include "Effect.h"

#include <vector>

#include <glm/glm.hpp>

namespace G2 
{
	class Effect;
	/** This class defines a generic Shader, written in any supported shading language.
	 * It is designed to hold as few informations as possible to be really lightweight.
	 * @created:	2014/02/10
	 * @author Andy Reimann <a.reimann@moorlands-grove.de>
	 */
	class Shader 
	{
		friend struct Effect::Builder; // only the UberShader is able to set and compile Shaders

		public:
			/** This constructs a new Shader.
			 */
			Shader();

			/** This function returns the bindable state of the Shader.
			 * @return True if the Shader is ready for usage, false if not.
			 */
			bool isBindable() { return mCompiled; }
			/** This function will try to bind the given Shader.
			 * @note Make sure to only bind Shaders, which are bindable!
			 */
			virtual void bind() = 0;
			/** This function will set the given Property on the Shader to the given value.
			 * @param property The Property to set
			 * @param value The value to set the Property to.
			 */
			virtual void setProperty(std::string const& property, glm::mat4 const& value) = 0;
			/** This function will set the given Property on the Shader to the given value.
			 * @param property The Property to set
			 * @param value The value to set the Property to.
			 */
			virtual void setProperty(std::string const& property, glm::mat3 const& value) = 0;
			/** This function will set the given Property on the Shader to the given value.
			 * @param property The Property to set
			 * @param value The value to set the Property to.
			 */
			virtual void setProperty(std::string const& property, glm::vec4 const& value) = 0;
			/** This function will set the given Property on the Shader to the given value.
			 * @param property The Property to set
			 * @param value The value to set the Property to.
			 */
			virtual void setProperty(std::string const& property, glm::vec3 const& value) = 0;
			/** This function will set the given Property on the Shader to the given value.
			 * @param property The Property to set
			 * @param value The value to set the Property to.
			 */
			virtual void setProperty(std::string const& property, glm::vec2 const& value) = 0;
			/** This function will set the given Property on the Shader to the given value.
			 * @param property The Property to set
			 * @param value The value to set the Property to.
			 */
			virtual void setProperty(std::string const& property, float value) = 0;
			/** This function will set the given Property on the Shader to the given value.
			 * @param property The Property to set
			 * @param value The value to set the Property to.
			 */
			virtual void setProperty(std::string const& property, int value) = 0;
			/** This function will set the MacroConditions, which are checked at runtime
			 * to find the Shader, which is best for rendering an Object.
			 * @param conditions A vector containing all MacroConditions.
			 */
			void setConditions(std::vector<MacroCondition> const&	conditions);
			/** This function will return the MacroConditions of the Shader.
			 * @return A vector containing all MacroConditions of this Shader.
			 */
			std::vector<MacroCondition> const& getConditions() const { return mConditions; }
			/** normal destructor
			 */
			virtual ~Shader();
		protected:
			/** This function will initialize all shader data, which can
			 * be initialized using the given ShaderMetaData.
			 * @param metaData The ShaderMetaData to use for initialization.
			 */
			void initWithMetaData(ShaderMetaData const& metaData);
			
			/** This function will compile a new Shader from the given shader code.
			 * @param vertexCode The Vertex-Shader code to use
			 * @param geometryCode The Geometry-Shader code to use or an empty string if no geometry shader should be used.
			 * @param fragmentCode The Fragment-Shader code to use
			 * @return True if the Shader compiled successfully, false if not.
			 */
			virtual bool compile(std::string const& vertexCode, std::string const& geometryCode, std::string const& fragmentCode) = 0;

			bool						mCompiled;		// True if the Shader is compiled, false if not.
			std::vector<MacroCondition>	mConditions;	// The conditions to pass to use this Shader 
	};
};