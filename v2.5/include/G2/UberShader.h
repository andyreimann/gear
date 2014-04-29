// GEAR 2.5 - Game Engine Andy Reimann - Author: Andy Reimann <andy@moorlands-grove.de>
// (c) 2014 GEAR 2.5
#pragma once
#include "LocationBinding.h"
#include "Property.h"
#include "AbstractShaderPart.h"
#include "Material.h"
#include "VertexArrayObject.h"
#include "ShaderMetaData.h"

#include <vector>
#include <memory>

namespace G2 {

	class Shader;

	namespace ShadingLanguage {
		enum Name {
			GLSL = 1,
			CG,
			UNKNOWN,
		};
	};
	/** This class manages all shader codes, conditions, macros and properties 
	 * needed to produce a set of Shader objects to use on different scenes and settings.
	 * It creates multiple permutations of a shader if requested and chooses the
	 * best fit for a request of a shader.
	 * An UberShader only holds compiled Shader objects.
	 * @created:	2014/02/09
	 * @author Andy Reimann <a.reimann@moorlands-grove.de>
	 */
	class UberShader {

		friend class UberShaderParser;
		friend class UberShaderBlockParser;
		friend class PropertiesBlockParser;
		friend class LocationBindingsBlockParser;
		friend class ShaderBlockParser;
	
		public:
			/** This constructs a new UberShader.
			 * The newly constructed UberShader has no compiled Shader object.
			 * Therefore it is a placeholder for no shader.
			 * @see G2::UberShaderParser::parse
			 */
			UberShader();
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
			/** This function will return all Shader.
			 * @return All Shader.
			 */
			std::vector<std::shared_ptr<Shader>> const& getAllShaders() const { return mShaderPermutations; };
			/** This function will return the ShadingLanguage. 
			* @return The current ShadingLanguage.
			*/
			ShadingLanguage::Name getShadingLanguage() { return mShadingLanguage; }
			/** This function will return the MetaData. 
			* @return The current MetaData.
			*/
			G2::ShaderMetaData const& getMetaData() { return mMetaData; }
		private:
			/** This function will add a new LocationBinding to the list of LocationBindings.
			 * @param locationBinding The LocationBinding to add.
			 */
			void add(LocationBinding const& locationBinding);
			/** This function will add a new Property to the list of Properties.
			 * @param property The Property to add.
			 */
			void add(Property const& property);
			/** This function will add a new shader part to the list of shader parts for 
			 * the vertex shader.
			 * @param part The shader code to add.
			 */
			void addVertexShaderPart(std::shared_ptr<AbstractShaderPart> const& part);
			/** This function will add a new shader part to the list of shader parts for 
			 * the fragment shader.
			 * @param part The shader code to add.
			 */
			void addFragmentShaderPart(std::shared_ptr<AbstractShaderPart> const& part);
			/** This function will build and compile all shader permutations, which 
			 * are created from all collected shader parts.
			 * @return The UberShader itself.
			 */
			UberShader& buildAndCompile();
			/** This function will set the shading language based on a given string representation.
			 * @param shadingLanguage Should be either GLSL or CG.
			 */
			void setShadingLanguage(std::string const& shadingLanguage);
			/** This function will return the MetaData. 
			* @return The current MetaData.
			*/
			G2::ShaderMetaData& getMetaDataReference() { return mMetaData; }
			
			ShadingLanguage::Name								mShadingLanguage;		// The shading language to use.
			std::vector<LocationBinding>						mLocationBindings;		// The available LocationBindings used by all Shaders
			std::vector<Property>								mProperties;			// The available Properties used by all Shaders
			std::vector<std::shared_ptr<AbstractShaderPart>>	mVertexShaderParts;		// The available parts of vertex shader code
			std::vector<std::shared_ptr<AbstractShaderPart>>	mFragmentShaderParts;	// The available parts of fragment shader code
			std::vector<std::shared_ptr<Shader>>				mShaderPermutations;	// The vector containing all available Shader-Permutations

			ShaderMetaData										mMetaData;				// The meta data containing all informations for efficient runtime checks
	};
};