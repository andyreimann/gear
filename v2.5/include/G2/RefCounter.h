// GEAR 2.5 - Game Engine Andy Reimann - Author: Andy Reimann <andy@moorlands-grove.de>
// (c) 2014 GEAR 2.5
#pragma once

namespace G2 
{
	/** This class implements a generic reference counting.
	 * A newly created RefCounter object has a start value
	 * of 1 references.
	 * @created:	2014/02/02
	 * @author Andy Reimann <a.reimann@moorlands-grove.de>
	 */
	template<class T = int, int start = 1>
	class RefCounter
	{
		public:
			RefCounter() 
				: count(start) 
			{}
			
			/** This function will increment the number of references.
			 */
			void incr() 
			{
				// Increment the reference count
				++count;
			}
			/** This function will decrement the number of references.
			 * @return The number of references after the decrement.
			 */
			int decr()
			{
				// Decrement the reference count and
				// return the reference count.
				return --count;
			}
		private:
			T count; //<! Reference count
	};
};