#pragma once

#include "Texture2D.h"
#include <memory>

namespace G2 
{
	/** This class defines...
	 * @created:	2014/06/26
	 * @author Andy Reimann <a.reimann@moorlands-grove.de>
	 */
	class NormalMapGenerator 
	{
		public:
			/** This function generates a normal map from a given height map.
			 * It uses Catmull-Rom interpolation to determine the slope at the pixels.
			 * The slope is then used to approximate the normal vector for every pixel.
			 * @param heightMap The given one channel height map.
			 * @param the width and height of the given height map.
			 * @param maxHeight The maximum height factor of the height map
			 * @return The generated normal texture as an RGB texture.
			 */
			static std::shared_ptr<Texture2D> generateFromHeightMap(unsigned char* heightMap, int size, float maxHeight = 255.f);

		private:
			/// This constructs a new NormalMapGenerator.
			NormalMapGenerator() {}
	};
};