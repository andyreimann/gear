// GEAR 2.5 - Game Engine Andy Reimann - Author: Andy Reimann <andy@moorlands-grove.de>
// (c) 2014 GEAR 2.5
#pragma once
#include "VersionTracker.h"

namespace G2 
{
	/** The ShaderCache class holds a cached Shader together
	 * with Version objects for a Material and a VertexArrayObject, which produced the cache.
	 * @created:	2014/02/13
	 * @author Andy Reimann <a.reimann@moorlands-grove.de>
	 */
	class ShaderCache 
	{
		public:
			/** This will return the currently cached Shader.
			 * @return The currently cached Shader.
			 */
			std::shared_ptr<Shader> const& getShader() 
			{
				return mShader;
			}
			/** This will update the cache with the given values.
			 * @param shader The Shader to cache.
			 * @param materialVersion The version of the Material
			 * @param vapVersion The version of the VertexArrayObject.
			 */
			void setShader(std::shared_ptr<Shader> const& shader, Version materialVersion, Version vaoVersion) 
			{
				mShader = shader;
				mMaterialVersion = materialVersion;
				mVAOVersion = vaoVersion;
			}
			/** This will check the cache with the given Versions.
			 * @param materialVersion The version of the Material
			 * @param vaoVersion The version of the VertexArrayObject.
			 * @return True if the cache is up to date, false if not.
			 */
			bool isCacheValid(Version materialVersion, Version vaoVersion) 
			{
				return mShader.get() != nullptr && materialVersion == mMaterialVersion && mVAOVersion == vaoVersion;
			}

		private:

			std::shared_ptr<Shader> mShader;			// The currently cached Shader
			Version					mVAOVersion;		// The version of the VertexArrayObject
			Version					mMaterialVersion;	// The version of the Material
	};
};