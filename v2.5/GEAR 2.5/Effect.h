#pragma once
#include "ResourceBuilder.h"
#include "AbstractShaderPart.h"
#include "LocationBinding.h"
#include "Property.h"
#include "ShaderMetaData.h"

namespace G2 
{
	namespace ShadingLanguage
	{
		enum Name
		{
			GLSL = 1,
			CG,
			UNKNOWN
		};
	};

	class Shader;
	/// This class holds all informations needed to describe and use an
	/// effect.
	/// @created:	2014/03/11
	/// @author Andy Reimann <a.reimann@moorlands-grove.de>
	class Effect 
	{
		public:

			struct Builder : public ResourceBuilder<Builder,Effect>
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
					 * the fragment shader.
					 * @param parts The shader code parts to add.
					 */
					void addFragmentShaderParts(std::vector<std::shared_ptr<AbstractShaderPart>> const& parts);
					/** This function will build and compile all shader permutations, which 
					 * are created from all collected shader parts.
					 * @return The UberShader itself.
					 */
					Effect::Builder& buildAndCompile();
			
					ShadingLanguage::Name								shadingLanguage;		// The shading language to use.
					std::vector<LocationBinding>						locationBindings;		// The available LocationBindings used by all Shader
					std::vector<Property>								properties;			// The available Properties used by all Shader
			
					std::vector<std::shared_ptr<AbstractShaderPart>>	vertexShaderParts;		// The available parts of vertex shader code
					std::vector<std::shared_ptr<AbstractShaderPart>>	fragmentShaderParts;	// The available parts of fragment shader code
					std::vector<std::shared_ptr<Shader>>				shaderPermutations;	// The vector containing all available Shader-Permutations


					ShaderMetaData										metaData;	


			};
			/// This constructs a new Effect.
			Effect();
			/// Copy constructor.
			Effect(Effect const& rhs);
			/// Basic assignment operator.
			Effect& operator=(Effect const& rhs);
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
			 */
			std::shared_ptr<Shader> getShader(Material const& material, VertexArrayObject const& vao);
			/// normal destructor
			~Effect();
		protected:
		private:

			std::vector<std::shared_ptr<Shader>>	mShaderPermutations;	// The vector containing all available Shader-Permutations

	};
};