#pragma once

namespace G2Core
{
	/** This class defines...
	 * @created:	2014/11/10
	 * @author Andy Reimann <a.reimann@moorlands-grove.de>
	 */
	struct TextureMetaData 
	{
		enum Status {
			OK = 0,
			INVALID_ARG = 1,
			FORMAT_NOT_SUPPORTED = 1,
		};

		/// This constructs a new TextureMetaData.
		TextureMetaData() :
			width(0),
			height(0),
			status(OK) {}

		unsigned int width;							// The width of the Texture
		unsigned int height;						// The height of the Texture
		G2Core::DataFormat::Base::Name baseFormat;	// The base format of the Texture

		Status status;			// The status of the action performed
		std::string error;		// An optional string containing any detailed information on errors
	};
};