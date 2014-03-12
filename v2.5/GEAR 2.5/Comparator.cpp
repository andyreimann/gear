// GEAR 2.5 - Game Engine Andy Reimann - Author: Andy Reimann <andy@moorlands-grove.de>
// (c) 2014 GEAR 2.5
#include "Comparator.h"

using namespace G2;

G2::Comparator::FunctionI
Comparator::toComparatorI(std::string const& comparator) 
{
	// parse comparator
	if(comparator == "==") 
	{	
		return Comparator::equals<int>;
	}
	else if(comparator == "<") 
	{
		return Comparator::lessThan<int>;
	}
	else if(comparator == "<=") 
	{
		return Comparator::lessThanOrEqual<int>;
	}
	else if(comparator == ">") 
	{
		return Comparator::greaterThan<int>;
	}
	else if(comparator == ">=") 
	{
		return Comparator::greaterThanOrEqual<int>;
	}
	// this special case is interpreted as '=='
	return Comparator::equals<int>;
}