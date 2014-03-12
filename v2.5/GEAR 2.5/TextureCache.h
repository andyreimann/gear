// GEAR 2.5 - Game Engine Andy Reimann - Author: Andy Reimann <andy@moorlands-grove.de>
// (c) 2014 GEAR 2.5
#pragma once

namespace G2 
{
	struct CachedTextureInfo {

	};
	/** This class implements caching for textures loaded from files.
	 * @created:	2014/02/17
	 * @author Andy Reimann <a.reimann@moorlands-grove.de>
	 */
	class TextureCache 
	{
		public:
			/** Get a reference to one single instance.
			 * @return A reference of one single instance.
			 */
			static TextureCache& getShared();
			/** Destroys the one single instance.
			 */
			static void destroy();
		protected:
		private:
			/** This constructs a new TextureCache.
			 */
			TextureCache() {}
			/** Copy constructor.
			 */
			TextureCache(TextureCache const& rhs) {}
			/** Basic assignment operator.
			 */
			TextureCache& operator=(TextureCache const& rhs) { return *this; }
			/** normal destructor
			 */
			~TextureCache();

			static TextureCache*				mInstance_;			// The one single instance
			
	};
};