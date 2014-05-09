#pragma once
#include "Property.h"
#include "Setting.h"
#include "LocationBinding.h"
#include "ShaderMetaData.h"
#include "Property.h"
#include "RenderTarget.h"
#include "NonCopyable.h"
#include "Material.h"

#include <memory>
#include <vector>
#include <unordered_map>
#include <string>

namespace G2 
{
	class Shader;
	class AbstractShaderPart;

	
	namespace PointOfView
	{
		enum Name {
			MAIN_CAMERA, // Default value
			LOCAL,
			POV_INVALID,
		};
		/** This function will parse the given string to the appropriate
		 * PointOfView enum value.
		 * @param name The name to parse.
		 * @return The parsed PointOfView enum value.
		 */
		PointOfView::Name getPointOfView(std::string const& name);
	};
	/// This class defines...
	/// @created:	2014/05/07
	/// @author Andy Reimann <a.reimann@moorlands-grove.de>
	class Pass : public NonCopyable
	{
		public:
			struct Builder
			{
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
					
				Sampler::Name										renderTargetSampler;
				RenderTargetType::Name								renderTargetType;
				std::unordered_map<std::string,Setting>				settings;
				std::vector<LocationBinding>						locationBindings;	// The available LocationBindings used by all Shader
				std::vector<Property>								properties;	
				std::vector<std::shared_ptr<AbstractShaderPart>>	vertexShaderParts;		// The available parts of vertex shader code
				std::vector<std::shared_ptr<AbstractShaderPart>>	fragmentShaderParts;	// The available parts of fragment shader code
				std::vector<std::shared_ptr<Shader>>				shaderPermutations;		// The vector containing all available Shader-Permutations
				
				ShaderMetaData										metaData;
			};
			/// This constructs a new Pass.
			/// @param shaderPermutations The valid compiled Shader permutations of this Pass
			/// @param settings All parsed settings for this Pass
			/// @param renderTargetProperty The property containing the informations about the RenderTarget.
			/// @param
			Pass(std::vector<std::shared_ptr<Shader>> const& shaderPermutations, 
				 std::unordered_map<std::string,Setting> const& settings, 
				 Sampler::Name renderTargetSampler, 
				 RenderTargetType::Name renderTargetType);
			
			/// Move ctor.
			Pass(Pass && rhs);
			/// Move ctor.
			Pass& operator=(Pass && rhs);
			/** This function will return the Shader, which passes the most MacroConditions
			 * This is the one, which fits best.
			 * @param material The Material, which should be used for decision making.
			 * @param vao The VertexArrayObject, which should be used for decision making.
			 * @return The Shader which fits best or a shared pointer pointing to nullptr,
			 * if no Shader was found.
			 */
			std::shared_ptr<Shader> getShader(Material const& material, VertexArrayObject const& vao) const;
			/** This function will return the RenderTarget. 
			 * @return The RenderTarget of the Pass.
			 */
			G2::RenderTarget const& getRenderTarget() const { return mRenderTarget; }
			/** This function will return the number of iterations, the Pass requests to be complete. 
			* @return The number of iterations, the Pass requests to be complete.
			*/
			int const& getNumRenderIterations() const { return mNumRenderIterations; }
			/** This function will return the point of view to use while rendering. 
			* @return The the point of view to use while rendering.
			*/
			PointOfView::Name const& getPov() const { return mPov; }
		protected:
			std::unordered_map<std::string,Setting> mSettings;				// The settings of the pass
			std::vector<std::shared_ptr<Shader>>	mShaderPermutations;	// The vector containing all available Shader-Permutations
			RenderTarget							mRenderTarget;			// The render target of the Pass
			PointOfView::Name						mPov;					// The point of view of the pass
			int										mNumRenderIterations;	// The number of iterations the pass needs to be completed
	};
};
