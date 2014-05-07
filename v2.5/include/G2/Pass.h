#pragma once
#include "Property.h"
#include "SettingsBlockParser.h"

#include <memory>
#include <vector>
#include <unordered_map>
#include <string>

namespace G2 
{
	class Shader;
	/// This class defines...
	/// @created:	2014/05/07
	/// @author Andy Reimann <a.reimann@moorlands-grove.de>
	class Pass 
	{
		public:
			/// This constructs a new Pass.
			Pass(
				std::vector<std::shared_ptr<Shader>> shaderPermutations, 
				std::vector<Property> const& globalProperties,
				std::unordered_map<std::string,Setting> settings);
		protected:
			
			std::vector<Property> mProperties; 
			std::unordered_map<std::string,Setting> mSettings; 


			std::vector<std::shared_ptr<Shader>>	mShaderPermutations;	// The vector containing all available Shader-Permutations
	};
};
