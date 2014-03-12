// GEAR 2.5 - Game Engine Andy Reimann - Author: Andy Reimann <andy@moorlands-grove.de>
// (c) 2014 GEAR 2.5
#include "IfElseMacro.h"

using namespace G2;

IfElseMacro::IfElseMacro(std::string const& resource, std::string const& comparator, std::string const& genericValue) 
	: mElseMacroAppended(false)
{
	mComparator = Comparator::toComparatorI(comparator);
	mMacroResource = MacroResource::toMacroResource(resource);

	// parse generic value
	mTargetValue = 1;
	if(genericValue.length() != 0) 
	{
		mTargetValue = std::atoi(genericValue.c_str());
	}
}

void
IfElseMacro::append(std::string const& part) 
{
	if(part == "#ELSE") {
		mElseMacroAppended = true;
		return;
	}
	if(!mElseMacroAppended) 
	{
		mIfCode.append("\n");
		mIfCode.append(part);
	}
	else 
	{
		mElseCode.append("\n");
		mElseCode.append(part);
	}
}

unsigned int
IfElseMacro::getNumPermutations() const 
{
	return 2;
}

std::string
IfElseMacro::getPermutedShaderPart(int index) const 
{
	return index == 0 ? mIfCode : mElseCode;
}

std::vector<MacroCondition>
IfElseMacro::getMacroConditions(int index) const 
{
	std::vector<MacroCondition> conditions;
	
	if(index == 0) 
	{
		// for if part, all conditions have to be met
		conditions.push_back(MacroCondition(mComparator,
											mTargetValue,
											mMacroResource));
	}
	// add no conditions for else part -> lower indices are preferred!
	return conditions;
}