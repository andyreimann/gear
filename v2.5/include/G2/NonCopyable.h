// GEAR 2.5 - Game Engine Andy Reimann - Author: Andy Reimann <andy@moorlands-grove.de>
// (c) 2014 GEAR 2.5
#pragma once

namespace G2
{
	class NonCopyable 
	{

		protected:
			NonCopyable() {}
			~NonCopyable() {}

		private:
			NonCopyable(NonCopyable const&) {}
			NonCopyable const& operator=(NonCopyable const&) {}
	};

};