#pragma once
#include "ResourceBuilder.h"
#include "UberShader.h"

namespace G2 
{
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
				
				UberShader				uberShader;
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