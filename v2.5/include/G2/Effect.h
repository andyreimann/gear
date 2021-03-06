#pragma once
#include "ResourceBuilder.h"
#include "AbstractShaderPart.h"
#include "LocationBinding.h"
#include "Property.h"
#include "ShaderMetaData.h"
#include "Pass.h"

#include <G2Core/VertexInputLayout.h>

namespace G2 
{
	class Shader;
	/// This class holds all informations needed to describe and use an Effect.
	/// Effect objects are used for rendering RenderComponents as well as post processing.
	/// @created:	2014/03/11
	/// @author Andy Reimann <a.reimann@moorlands-grove.de>
	class Effect 
	{
		public:
			struct Builder : public ResourceBuilder<Builder, std::shared_ptr<Effect>>
			{
				std::shared_ptr<Effect> buildResource();

				friend class UberShaderParser;
				friend class UberShaderBlockParser;
				friend class PropertiesBlockParser;
				friend class LocationBindingsBlockParser;
				friend class ShaderBlockParser;
	
					/** This constructs a new UberShader.
					 * The newly constructed UberShader has no compiled Shader object.
					 * Therefore it is a placeholder for no shader.
					 * @see G2::UberShaderParser::parse
					 */
					Builder();

					/** This function will set the shading language based on a given string representation.
					 * @param shadingLanguage Should be either GLSL or CG.
					 */
					void setShadingLanguage(std::string const& shadingLanguage);
					/** This function will add a list of new shader part to the list of shader parts for 
					 * the vertex shader.
					 * @param parts The shader code parts to add.
					 */
					void addVertexShaderParts(std::vector<std::shared_ptr<AbstractShaderPart>> const& parts);
					/** This function will add a list of new shader part to the list of shader parts for 
					 * the geometry shader.
					 * @param parts The shader code parts to add.
					 */
					void addGeometryShaderParts(std::vector<std::shared_ptr<AbstractShaderPart>> const& parts);
					/** This function will add a list of new shader part to the list of shader parts for 
					 * the fragment shader.
					 * @param parts The shader code parts to add.
					 */
					void addFragmentShaderParts(std::vector<std::shared_ptr<AbstractShaderPart>> const& parts);
					/** This function will build and compile all shader permutations, which 
					 * are created from all collected shader parts.
					 * @return The UberShader itself.
					 */
					Effect::Builder& buildAndCompile();

					static bool compileAndApplyMetaData(std::string const& shadingLanguage, std::string const& vertexShaderCode, std::string const& geometryShaderCode, std::string const& fragmentShaderCode, ShaderMetaData const& shaderMetaData, G2Core::VertexInputLayout const& vertexInputLayout, std::shared_ptr<Shader> const& shader);
			
					std::string											shadingLanguage;	// The shading language to use.
					std::vector<LocationBinding>						locationBindings;	// The available LocationBindings used by all Shader
					std::vector<Property>								properties;			// The available Properties used by all Shader
					std::unordered_map<std::string, Setting>			settings;			// The settings of the Shader object
			
					std::vector<std::shared_ptr<AbstractShaderPart>>	vertexShaderParts;	// The available parts of vertex shader code
					std::vector<std::shared_ptr<AbstractShaderPart>>	geometryShaderParts;// The available parts of geometry shader code
					std::vector<std::shared_ptr<AbstractShaderPart>>	fragmentShaderParts;// The available parts of fragment shader code
					std::vector<std::shared_ptr<Shader>>				shaderPermutations;	// The vector containing all available Shader-Permutations


					std::string											vertexShaderVersion;
					std::string											geometryShaderVersion;
					std::string											fragmentShaderVersion;

					
					ShaderMetaData										metaData;	

					std::vector<Pass::Builder>							passes;				// The vector containing all available Shader-Permutations

			};
			Effect(std::unordered_map<std::string, Setting> const& settings);

			/** Returns a value of a setting coming from a Settings block of an Effect file.
			 * @param name The name of the Setting to retrieve.
			 * @param defaultValue The default value to return in case the setting is not present.
			 */
			G2::Setting const& getSetting(std::string const& name, std::string const& defaultValue = "");
			/** This function returns the state of the UberShader.
			 * @return True if the UberShader has some compiled Shader, false if not.
			 */
			bool hasCompiledShaders() const;
			/** This function will return the Shader, which passes the most MacroConditions
			 * This is the one, which fits best.
			 * @param material The Material, which should be used for decision making.
			 * @param vao The VertexArrayObject, which should be used for decision making.
			 * @return The Shader which fits best or a shared pointer pointing to nullptr,
			 * if no Shader was found.
			 * @note By passing in no material and/or no vao, the first shader will be returned as default.
			 */
			std::shared_ptr<Shader> getShader(Material const* material = nullptr, VertexArrayObject const* vao = nullptr) const;
			/** This function will return the Passes. 
			* @return The current Passes.
			*/
			std::vector<Pass> const& getPasses() const { return mPasses; }
		protected:
		private:

			std::unordered_map<std::string, Setting> mSettings;				// The settings of the Effect file
			std::vector<std::shared_ptr<Shader>>	mShaderPermutations;	// The vector containing all available Shader-Permutations
			std::vector<Pass>						mPasses;				// The vector of passes for this Effect

	};
};