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
			std::shared_ptr<Texture2D> importResource(
				std::string const& fileName, 
				unsigned minFilter, 
				unsigned magFilter, 
				bool compress = false, 
				WrapMode::Name wrapS = WrapMode::REPEAT, 
				WrapMode::Name wrapT = WrapMode::REPEAT);

			std::pair<std::string,std::shared_ptr<Texture2D::Builder>> produceResourceBuilder(
				std::string const& fileName, 
				unsigned minFilter, 
				unsigned magFilter, 
				bool compress = false, 
				WrapMode::Name wrapS = WrapMode::REPEAT,
				WrapMode::Name wrapT = WrapMode::REPEAT);
	};
};