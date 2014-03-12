// GEAR 2.5 - Game Engine Andy Reimann - Author: Andy Reimann <andy@moorlands-grove.de>
// (c) 2014 GEAR 2.5
#pragma once
#include "MacroCondition.h"

#include <string>
#include <vector>

namespace G2 
{
	/** This class defines the API for a shader code part.
	 * @created:	2014/02/09
	 * @author Andy Reimann <a.reimann@moorlands-grove.de>
	 */
	class AbstractShaderPart 
	{
		public:
			/** This constructs a new AbstractShaderPart.
			 */
			AbstractShaderPart();
			/** Returns the number of permutations requested by this shader part.
			 * @return The number of permutations requested by this shader part.
			 */
			virtual unsigned int getNumPermutations() const { return 0; };
			/** Returns the permute shader part at the given index.
			 * @return The permute shader part at the given index.
			 */
			virtual std::string getPermutedShaderPart(int index) const { return ""; }
			/** Returns the MacroConditions, which have to be passed to use the 
			 * permuted shader part at the given index.
			 * @return The vector of MacroConditions, which have to be passed to use the shader permutation at the given index
			 * 
			 * @note The final list of Shaders is sorted, so that the permutations
			 * with the lower indices are checked before the higher ones.
			 */
			virtual std::vector<MacroCondition> getMacroConditions(int index) const { return std::vector<MacroCondition>(); }
			/** This function will append the given part to the shader part.
			 * @param part The part to append.
			 */
			virtual void append(std::string const& part) { };

		protected:

			
	};
};