// GEAR 2.5 - Game Engine Andy Reimann - Author: Andy Reimann <andy@moorlands-grove.de>
// (c) 2014 GEAR 2.5
#pragma once
#include "AbstractShaderPart.h"

#include <string>

namespace G2 
{
	/** This class holds some shader code, which is nothing more than valid
	 * shader code without any conditions or something else.
	 * @created:	2014/02/09
	 * @author Andy Reimann <a.reimann@moorlands-grove.de>
	 */
	class SimpleShaderPart : public AbstractShaderPart 
	{
		public:
			/** This constructs a new SimpleShaderPart.
			 */
			SimpleShaderPart();
			/** This function will always return 1, since a SimpleShaderPart will always produce 1 shader
			 * permutation.
			 * @return 1;
			 */
			virtual unsigned int getNumPermutations() const;
			/** This function will return the appended code.
			 * @param index - not used
			 */
			virtual std::string getPermutedShaderPart(int index) const;
			/** This function will append the given part to the SimpleShaderPart.
			 * @param part The part to add.
			 */
			virtual void append(std::string const& part);

		protected:

			std::string			mPart;	// The part of Shader-Code

	};
};