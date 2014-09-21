// GEAR 2.5 - Game Engine Andy Reimann - Author: Andy Reimann <andy@moorlands-grove.de>
// (c) 2014 GEAR 2.5
#pragma once
#include "Importer.h"
#include "Texture2D.h"

namespace G2 
{
	/** This class implements the generic resource importer interface
	 * to provide an importer for Texture2D objects from files.
	 * @created:	2014/02/18
	 * @author Andy Reimann <a.reimann@moorlands-grove.de>
	 */
	class TextureImporter : public Importer<TextureImporter,Texture2D, Texture2D::Builder>
	{
		public:

			/** This function will import a given texture file.
			 * @note Don't use this function directly, use import() instead!
			 * @param fileName the name of the texture file to import.
			 * @param minFilter the min filter to use. Valid values are GL_NEAREST, GL_LINEAR, GL_NEAREST_MIPMAP_NEAREST, GL_LINEAR_MIPMAP_NEAREST, GL_NEAREST_MIPMAP_LINEAR, GL_LINEAR_MIPMAP_LINEAR.
			 * @param magFilter The mag filter to use. Valid values are GL_NEAREST, GL_LINEAR. The magnification filter can't specify the use of mipmaps; only the minification filter can do that.
			 * @param compress Flag indicating whether you want the Texture to be loaded using a compressed format.
			 * @param wrapS The wrap mode to use for the S coordinates.
			 * @param wrapT The wrap mode to use for the T coordinates.
			 * @param internalFormat The internal format you want to use for the texture. Use any allowed OpenGL format or -1, if you don't want to change the format read from the texture file itself.
			 * @return The loaded texture instance, or null if the texture could not be loaded.
			 */
			std::shared_ptr<Texture2D> importResource(
				std::string const& fileName, 
				G2Core::FilterMode::Name minFilter, 
				G2Core::FilterMode::Name magFilter, 
				bool compress = false, 
				G2Core::WrapMode::Name wrapS = G2Core::WrapMode::REPEAT, 
				G2Core::WrapMode::Name wrapT = G2Core::WrapMode::REPEAT,
				G2Core::DataFormat::Name internalFormat = G2Core::DataFormat::UNKNOWN);

			std::pair<std::string,std::shared_ptr<Texture2D::Builder>> produceResourceBuilder(
				std::string const& fileName, 
				G2Core::FilterMode::Name minFilter, 
				G2Core::FilterMode::Name magFilter, 
				bool compress = false, 
				G2Core::WrapMode::Name wrapS = G2Core::WrapMode::REPEAT,
				G2Core::WrapMode::Name wrapT = G2Core::WrapMode::REPEAT,
				G2Core::DataFormat::Name internalFormat = G2Core::DataFormat::UNKNOWN);
	};
};