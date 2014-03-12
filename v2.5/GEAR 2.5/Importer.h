// GEAR 2.5 - Game Engine Andy Reimann - Author: Andy Reimann <andy@moorlands-grove.de>
// (c) 2014 GEAR 2.5
#pragma once
#include "ResourceBuilder.h"

#include <unordered_map>
#include <string>

namespace G2 
{
	/** This class provides a generic interface for importing any kind of 
	 * resource using the Builder Pattern.
	 * This Import System provides a generic way to load, cache and manage 
	 * resources with a maximum flexibility and much cleaner code, because the creation
	 * process is separated from the runtime code of the finally created resource.
	 * This also results in less runtime code you have to maintain.
	 * @created:	2014/02/18
	 * @author Andy Reimann <a.reimann@moorlands-grove.de>
	 */
	template<typename CONCRETE_IMPORTER, typename RESOURCE_TYPE, typename RESOURCE_BUILDER_TYPE>
	class Importer 
	{
		public:
			/** This generic function will call the importResource function with the given variadic template.
			 * @param args The variadic template, which is passed to the derived importers importResource function.
			 * @return A shared pointer to the loaded resource or pointing to nullptr, if some error occurred during the 
			 * resource import process.
			 */
			template<typename ... ARGS>
			std::shared_ptr<RESOURCE_TYPE> import(ARGS ... args) 
			{
				return ((CONCRETE_IMPORTER*)this)->importResource(args ...);
			}
		protected:

			std::unordered_map<std::string,std::shared_ptr<RESOURCE_BUILDER_TYPE>> mCache; // The cache of already imported resources
	};
};