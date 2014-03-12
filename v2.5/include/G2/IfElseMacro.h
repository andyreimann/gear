// GEAR 2.5 - Game Engine Andy Reimann - Author: Andy Reimann <andy@moorlands-grove.de>
// (c) 2014 GEAR 2.5
#pragma once
#include "AbstractShaderPart.h"
#include "Comparator.h"
#include "MacroResource.h"

namespace G2 
{
	/** This class holds all informations for an If-Else Macro read from an UberSHader:
		Variant 1:
		#IF [RESOURCE] = [VALUE]
		// some valid shader code
		#ENDIF
		Variant 2:
		#IF [RESOURCE] = [VALUE]
			// some valid shader code 
		#ELSE
			// some valid shader code 
		#ENDIF
	 * @created:	2014/02/09
	 * @author Andy Reimann <a.reimann@moorlands-grove.de>
	 */
	class IfElseMacro : public AbstractShaderPart 
	{
		public:
			/** This constructs a new IfElseMacro.
			 */
			IfElseMacro(std::string const& resource, std::string const& comparator, std::string const& genericValue);
			/** This function will always return 2, since an if-else macro will always produce 2 shader
			 * permutations.
			 * @return 2;
			 */
			virtual unsigned int getNumPermutations() const;
			/** This function will return the permutation registered at the given index.
			 * @return At index 0, the code in the if scope is returned, at index 1, the code
			 * in the else scope is returned if available.
			 */
			virtual std::string getPermutedShaderPart(int index) const;
			/** This function will append the given part to the ShaderPart.
			 * @param part The part to add
			 * @note If part is '#ELSE', it is dismissed and the IfElseMacro will start appending to the else shader part!
			 */
			virtual void append(std::string const& part);
			/** Returns the MacroConditions, which have to be passed to use the 
			 * permuted shader part at the given index.
			 * @return The vector of MacroConditions, which have to be passed to use the shader permutation at the given index
			 */
			virtual std::vector<MacroCondition> getMacroConditions(int index) const;
		protected:

			MacroResource::Name		mMacroResource;			// The resource, which is monitored by this IfElseMacro
			Comparator::FunctionI	mComparator;			// Pair of a comparator function and the rhs parameter when calling that function
			int						mTargetValue;			// The value the condition resource should be equal to
			std::string				mIfCode;				// The code to use if the condition does match
			std::string				mElseCode;				// The code to use if the condition does not match
			bool					mElseMacroAppended;		// True if the #ELSE macro was already passed to append(), false if not.

		};
};