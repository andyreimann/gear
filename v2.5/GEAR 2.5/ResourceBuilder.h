// GEAR 2.5 - Game Engine Andy Reimann - Author: Andy Reimann <andy@moorlands-grove.de>
// (c) 2014 GEAR 2.5
#pragma once
#include <memory>

namespace G2 
{
	/** This class defines the interface for the Importer System.
	 * Every importer class provides a builder class, which is derived 
	 * from this class to perfectly fit into the Importer System of the engine.
	 * It provides a generic and extensible interface for dynamically loading
	 * resources of any different kind with a maximum flexibility over the creation process.
	 * @created:	2014/02/18
	 * @author Andy Reimann <a.reimann@moorlands-grove.de>
	 */
	template<typename DERIVED_BUILDER,typename RESOURCE>
	class ResourceBuilder 
	{
		public:
			
			/** This generic function will call the buildResource function with the given variadic template
			 * to create an instance of the managed resource from a previously created builder.
			 * @param args The variadic template, which is passed to the derived builders buildResource function.
			 * @return A shared pointer to the loaded resource or pointing to nullptr, if some error occurred during the 
			 * resource build process.
			 */
			template<typename ... ARGS>
			std::shared_ptr<RESOURCE> build(ARGS ... args) {
				return ((DERIVED_BUILDER*)this)->buildResource(args ...);
			}

			virtual ~ResourceBuilder() = 0 {}
	};
};