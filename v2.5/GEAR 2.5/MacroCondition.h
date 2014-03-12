// GEAR 2.5 - Game Engine Andy Reimann - Author: Andy Reimann <andy@moorlands-grove.de>
// (c) 2014 GEAR 2.5
#pragma once
#include "Comparator.h"
#include "MacroResource.h"
#include "Material.h"
#include "VertexArrayObject.h"

namespace G2 
{
	/** This class holds informations about a single MacroCondition.
	 ** A MacroCondition is a combination of a comparator function, a target value and a MacroResource to check against.
	 * @created:	2014/02/10
	 * @author Andy Reimann <a.reimann@moorlands-grove.de>
	 */
	class MacroCondition 
	{
		public:
			/** This constructs a new MacroCondition.
			 * @param comparatorFunc The comparator function pointer to invoke for the requirement check.
			 * @param targetValue The target value to compare the runtime value
			 * @param macroResource The MacroResource to check at runtime
			 */
			MacroCondition(Comparator::FunctionI comparatorFunc, 
						   int targetValue, 
						   MacroResource::Name macroResource);
			/** This function will run the check of the Requirement.
			 * @return True if the check passes, false if not.
			 */
			bool check(Material const& material, VertexArrayObject const& vao) const;

		private:
			Comparator::FunctionI	mComparatorFunction;	// The comparator function pointer to invoke for the check
			int						mTargetValue;			// The target value to compare the runtime value
			MacroResource::Name		mMacroResource;			// The MacroResource to check at runtime
	};
};