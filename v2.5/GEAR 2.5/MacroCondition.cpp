// GEAR 2.5 - Game Engine Andy Reimann - Author: Andy Reimann <andy@moorlands-grove.de>
// (c) 2014 GEAR 2.5
#include "MacroCondition.h"

using namespace G2;

MacroCondition::MacroCondition(Comparator::FunctionI comparatorFunc, 
							   int targetValue, 
							   MacroResource::Name macroResource) 
	: mComparatorFunction(comparatorFunc),
	mTargetValue(targetValue),
	mMacroResource(macroResource)
{ }

bool
MacroCondition::check(Material const& material, VertexArrayObject const& vao) const 
{
	assert(false);
	return true;
}