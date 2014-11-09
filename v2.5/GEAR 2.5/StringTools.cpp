// GEAR 2.5 - Game Engine Andy Reimann - Author: Andy Reimann <andy@moorlands-grove.de>
// (c) 2014 GEAR 2.5
#include "StringTools.h"

using namespace G2;
using namespace Tools;

std::string 
String::trim( std::string const& str ) {
	std::string buffer;
	int len = (int)str.length();
	int spaces = 0;
	int i;
	char c = 0;
	for(i = 0; i < len; ++i) {
		c = str.at(i);
		// test whether one of the chars is a whitespace or not
		if(c != 0x09 && c != 0x0a && c != 0x0c && c != 0x0d) {
			if( (i == 0 || i == len-1) && c == 0x20) {
				++spaces;
			}
			if(c == 0x20 && spaces < 1) {
				++spaces;
				buffer.push_back(c);
			}
			else if(c != 0x20) {
				spaces = 0;
				buffer.push_back(c);
			}
		}
	}
	return buffer;
}

bool
G2::Tools::String::endsWith(std::string const& str, std::string const& suffix)
{
	if (suffix.size() > str.size())
	{
		return false;
	}
	return std::equal(str.begin() + str.size() - suffix.size(), str.end(), suffix.begin());
}