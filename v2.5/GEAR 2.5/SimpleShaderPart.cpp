// GEAR 2.5 - Game Engine Andy Reimann - Author: Andy Reimann <andy@moorlands-grove.de>
// (c) 2014 GEAR 2.5
#include "SimpleShaderPart.h"
#include <math.h>

using namespace G2;

SimpleShaderPart::SimpleShaderPart()
{ }

void
SimpleShaderPart::append(std::string const& part) 
{
	mPart.append("\n");
	mPart.append(part);
}

unsigned int
SimpleShaderPart::getNumPermutations() const 
{
	return std::min<unsigned int>(1,(unsigned int)mPart.length());
}

std::string
SimpleShaderPart::getPermutedShaderPart(int index) const 
{
	return mPart;
}