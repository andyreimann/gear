// GEAR 2.5 - Game Engine Andy Reimann - Author: Andy Reimann <andy@moorlands-grove.de>
// (c) 2014 GEAR 2.5
#pragma once
#include "Sampler.h"
#include "VersionTracker.h"

#include <glm/glm.hpp>

#include <map>
#include <memory>

namespace G2 
{

	struct MaterialUBOStructure
	{
		glm::vec4 ambient;
		glm::vec4 diffuse;
		glm::vec4 specular;
		float shininess;
	};

	class Texture;
	/** This class holds all informations needed to describe a Material used for rendering VertexArrayObjects.
	 * @created:	2014/01/22
	 * @author Andy Reimann <a.reimann@moorlands-grove.de>
	 */
	class Material : public VersionTracker
	{
		friend class RenderComponent;
		public:
			/** This constructs a new Material with default values as following:
			 * ambient(0.2,0.2,0.2,1.0)
			 * diffuse(1.0,1.0,1.0,1.0)
			 * specular(0.0,0.0,0.0,1.0)
			 * shininess(1)
			 */
			Material();
			/** This constructs a new Material with the given ambient material term.
			* @param ambient The current ambient material term.
			 */
			Material(glm::vec4 const& ambient);
			/** Move ctor.
			 */
			Material(Material && rhs);
			/** Move assignment operator.
			 */
			Material& operator=(Material && rhs);
			/** Equality operator.
			 */
			bool operator==(Material const& rhs);
			/** This function will return the ambient material term. 
			* @return The current ambient material term.
			*/
			glm::vec4 const& getAmbient() const { return mAmbient; }
			/** This function will set the ambient material term to the given value.
			* @param ambient The current ambient material term.
			* @return The Material itself.
			*/
			Material& setAmbient(glm::vec4 const& ambient);
			/** This function will return the diffuse material term. 
			* @return The current diffuse material term.
			*/
			glm::vec4 const& getDiffuse() const { return mDiffuse; }
			/** This function will set the diffuse material term to the given value.
			* @param diffuse The current diffuse material term.
			* @return The Material itself.
			*/
			Material& setDiffuse(glm::vec4 const& diffuse);
			/** This function will set the ambient and diffuse material term to the given value.
			* @param color The ambient and diffuse material term.
			* @return The Material itself.
			*/
			Material& setAmbientAndDiffuse(glm::vec4 const& color);
			/** This function will return the specular material term. 
			* @return The current specular material term.
			*/
			glm::vec4 const& getSpecular() const { return mSpecular; }
			/** This function will set the specular material term to the given value.
			* @param specular The current specular material term.
			* @return The Material itself.
			*/
			Material& setSpecular(glm::vec4 const& specular);
			/** This function will return the shininess for the specular highlights. 
			* @return The current shininess for the specular highlights.
			*/
			float const& getShininess() const { return mShininess; }
			/** This function will set the shininess for the specular highlights to the given value.
			* @param shininess The current shininess for the specular highlights.
			* @return The Material itself.
			*/
			Material& setShininess(float const& shininess);
			/** Returns the transparency mode of the mesh.
			 * @return True if the Material is transparent, false if not.
			 */
			bool isTransparent() const;
			/** This function will return the texture of the given sampler. 
			 * @param sampler The sampler to set the texture for.
			 * @return The texture of the given sampler.
			 */
			std::shared_ptr<Texture> const& getTexture(Sampler::Name const& sampler) { return mTextures[sampler]; }
			/** This function will set the texture of the given sampler to the given value.
			 * A Material can have multiple Texture objects assigned to different Sampler values.
			 * The Textures are then bound to the predefined texture slots for the given Sampler.
			 * @param sampler The sampler to set the texture for.
			 * @param texture The texture of the given sampler.
			 */
			void setTexture(Sampler::Name const& sampler, std::shared_ptr<Texture> const& texture) { mTextures[sampler] = texture; }
			/** This function will return the Textures. 
			* @return The current Textures.
			*/
			std::map<Sampler::Name,std::shared_ptr<Texture>> const& getTextures() const { return mTextures; }

		private:
			void _updateTransparencyFlag();
			/** This function will set the entity id the Material is connected to.
			 * @note It is called as soon as a RenderComponent allocates some vertex array objects the first time.
			 * This is ok, since a default constructed RenderComponent does not have any renderable vertex array objects and thus no connection is needed.
			 * @param entityId The id of the entity, the Material is connected with.
			 */
			void _connectToEntityId(unsigned int entityId);

			// private members
			unsigned int			mEntityId;		// The id of the Entity, the Material is connected to.
			bool					mIsTransparent; // The flag for the transparency of the Material
			glm::vec4				mAmbient;		// The ambient material term
			glm::vec4				mDiffuse;		// The diffuse material term
			glm::vec4				mSpecular;		// The specular material term
			float					mShininess;		// The shininess for the specular highlights
			
			std::map<Sampler::Name,std::shared_ptr<Texture>> mTextures;	// The diffuse map of the Material
			bool					mChanged;
	};
};