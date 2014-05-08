// GEAR 2.5 - Game Engine Andy Reimann - Author: Andy Reimann <andy@moorlands-grove.de>
// (c) 2014 GEAR 2.5
#pragma once
#include "AbstractShaderPart.h"
#include "SettingsBlockParser.h"
#include "PassesBlockParser.h"
#include "Effect.h"
#include "LocationBindingsBlockParser.h"
#include "PropertiesBlockParser.h"

#include <unordered_map>

namespace G2 
{
	class FileResource;
	struct ShaderMetaData;

	enum ShaderBlockState {
		WAITING_FOR_SHADER_CODE = 1,
		READING_SETTINGS_BLOCK,
		READING_PROPERTIES_BLOCK,
		READING_LOCATIONBINDINGS_BLOCK,
		READING_PASSES_BLOCK,
		READING_VERTEX_SHADER,
		READING_FRAGMENT_SHADER,
	};
	/** This class takes care of loading the content of an Shader block
	 * inside of an UberShader file.
	 * @created:	2014/02/09
	 * @author Andy Reimann <a.reimann@moorlands-grove.de>
	 */
	class ShaderBlockParser 
	{
		public:
			/** This constructs a new ShaderBlockParser.
			 */
			ShaderBlockParser(Effect::Builder* builder, FileResource* file);
			void parse(ShaderMetaData* shaderMetaData);
			
			/** This function will return the parsed VertexShaderParts. 
			* @return The current VertexShaderParts.
			*/
			std::vector<std::shared_ptr<AbstractShaderPart>> const& getVertexShaderParts() const 
			{ 
				return mVertexShaderParts; 
			}
			
			/** This function will return the parsed FragmentShaderParts. 
			* @return The parsed FragmentShaderParts.
			*/
			std::vector<std::shared_ptr<AbstractShaderPart>> const& getFragmentShaderParts() const 
			{ 
				return mFragmentShaderParts; 
			}
			
			/// This function will return the Setting for a given name.
			/// @param name The name of the Setting object to get the Settings for.
			/// @return The Setting object registered under the given name or a default settings object
			/// if no Setting object is registered.
			Setting getSetting(std::string const& name) const
			{
				return mSettingsBlockParser.getSetting(name);
			}

			PassesBlockParser const& getPassesBlockParser() const
			{
				return mPassesBlockParser;
			}

			SettingsBlockParser const& getSettingsBlockParser() const
			{
				return mSettingsBlockParser;
			}
			/** This function will return the LocationBindingBlockParser. 
			* @return The current LocationBindingBlockParser.
			*/
			G2::LocationBindingsBlockParser const& getLocationBindingBlockParser() const { return mLocationBindingBlockParser; }
			/** This function will return the PropertiesBlockParser. 
			* @return The current PropertiesBlockParser.
			*/
			G2::PropertiesBlockParser const& getPropertiesBlockParser() const { return mPropertiesBlockParser; }
		protected:
		private:

			void parseLine(std::string const& line, std::stringstream& lineStr);

			void parseIncludeMacro(std::string const& line);


			void flushShaderPart(std::shared_ptr<AbstractShaderPart> newPart);
			
			
			std::shared_ptr<AbstractShaderPart> getInclude(std::string const& resource) const;



			static std::unordered_map<std::string,std::shared_ptr<AbstractShaderPart>> initDefaultIncludes();

			Effect::Builder*									mBuilder;
			FileResource*										mFile;
			unsigned int										mCurrentState;
			
			SettingsBlockParser									mSettingsBlockParser;
			PassesBlockParser									mPassesBlockParser;
			LocationBindingsBlockParser							mLocationBindingBlockParser;
			PropertiesBlockParser								mPropertiesBlockParser;
			std::shared_ptr<AbstractShaderPart>					mCurrentShaderPart;

			std::vector<std::shared_ptr<AbstractShaderPart>>	mVertexShaderParts;		// The available parts of vertex shader code
			std::vector<std::shared_ptr<AbstractShaderPart>>	mFragmentShaderParts;	// The available parts of fragment shader code
			

			static std::unordered_map<std::string,std::shared_ptr<AbstractShaderPart>> gDefaultIncludes; // The registered default includes
	};
};