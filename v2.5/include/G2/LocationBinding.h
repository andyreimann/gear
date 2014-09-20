// GEAR 2.5 - Game Engine Andy Reimann - Author: Andy Reimann <andy@moorlands-grove.de>
// (c) 2014 GEAR 2.5
#pragma once
#include "VertexArrayObject.h"

#include <G2Core/VertexInputLayout.h>

#include <string>

namespace G2 
{
	/** This class holds all informations needed to describe a location binding
	 * used in a Shader and parsed from an UberShader.
	 * Location bindings are compiled to layout qualifiers 
	 * Location bindings are 
	 * @created:	2014/02/09
	 * @author Andy Reimann <a.reimann@moorlands-grove.de>
	 */
	class LocationBinding 
	{
		public:
			LocationBinding() {}
			/** This constructs a new LocationBinding.
			 * @param semantic The semantic of the location binding
			 * @param name The name of the location binding variable
			 * @param niceName The human readable name of the location binding variable (not used in final shader code)
			 * @param dataType The datatype of the location binding.
			 */
			LocationBinding(G2Core::Semantics::Name semantic, std::string const& name, std::string const& niceName, std::string const& dataType);
			/** This function will return the shader code produced by this LocationBinding object.
			 * @return The shader code produced by this LocationBinding object.
			 */
			std::string const& getShaderCode() const { return mShaderCode; };
			/** Calculates the VertexInputElement of this LocationBinding object.
			 * This is always needed when rendering a VertexBuffer together with a Shader.
			 @return The VertexInputElement of this LocationBinding object.
			 */
			G2Core::VertexInputElement getVertexInputElement() const;

			/** Calculates the G2Core::SemanticDataTypes::Name object by the give LocationBinding datatype.
			 */
			static G2Core::SemanticDataTypes::Name getSemanticDataTypeByLocationBindingDataType(std::string const& locationBindingDataType);
		private:

			G2Core::Semantics::Name mSemantic;	// The location of the location binding
			std::string		mName;				// The name of the location binding variable
			std::string		mNiceName;			// The human readable name of the location binding variable (not used in final shader code)
			std::string		mDataType;			// The datatype of the location binding
			std::string		mShaderCode;		// The shader code produced by this LocationBinding object
	};
};
