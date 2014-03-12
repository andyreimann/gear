// GEAR 2.5 - Game Engine Andy Reimann - Author: Andy Reimann <andy@moorlands-grove.de>
// (c) 2014 GEAR 2.5
#pragma once

namespace G2 
{
	class NonInstanceable 
	{
		private:
			NonInstanceable() {}
			~NonInstanceable() {}
			NonInstanceable(NonInstanceable const&) {}
			NonInstanceable const& operator=(NonInstanceable const&) {}
	};
};