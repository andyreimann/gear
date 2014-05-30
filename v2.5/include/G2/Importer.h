// GEAR 2.5 - Game Engine Andy Reimann - Author: Andy Reimann <andy@moorlands-grove.de>
// (c) 2014 GEAR 2.5
#pragma once
#include "ResourceBuilder.h"

#include <unordered_map>
#include <map>
#include <string>
#include <mutex>

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
			 * @note Thread safe.
			 */
			template<typename ... ARGS>
			std::shared_ptr<RESOURCE_TYPE> import(ARGS ... args) 
			{
				cache(args ...);
				mResourceMutex.lock();
				std::shared_ptr<RESOURCE_TYPE> resource =  ((CONCRETE_IMPORTER*)this)->importResource(args ...);
				mResourceMutex.unlock();
				if(!mCachingEnabled)
				{
					clearCache();
				}
				return resource;
			}
			/** This generic function will call the importResource function with the given variadic template.
			 * @param args The variadic template, which is passed to the derived importers importResource function.
			 * @return A shared pointer to the loaded resource or pointing to nullptr, if some error occurred during the 
			 * resource import process.
			 * @note Thread safe.
			 */
			template<typename ... ARGS>
			void cache(ARGS ... args) 
			{
				if(!mCachingEnabled)
				{
					return;
				}
				std::lock_guard<std::mutex> lock(mResourceMutex);
				std::pair<std::string,std::shared_ptr<RESOURCE_BUILDER_TYPE>> cacheEntry = ((CONCRETE_IMPORTER*)this)->produceResourceBuilder(args ...);
				if(mCachingEnabled && cacheEntry.second.get() != nullptr)
				{
					mCache[cacheEntry.first] = cacheEntry.second;
				}
			}
			/** Sets the caching mode.
			 * @param enabled True if caching should be enabled, false if not.
			 */
			void setCachingMode(bool enabled)
			{
				mCachingEnabled = enabled;
			}
			/** Checks if a given resource is already cached.
			 * @param name The name of the resource to check.
			 * @return True if the resource is cached, false if not.
			 * @warning This function is not thread safe!
			 */
			bool isCached(std::string const& name)
			{
				return mCache.find(name) != mCache.end();
			}
			/** Clears all cached resources.
			 * @note Thread safe.
			 */
			void clearCache()
			{
				std::lock_guard<std::mutex> lock(mResourceMutex);
				mCache.clear();
			}
			/** Locks a mutex to make access to resources thread safe.
			 * Normally the Importer takes care of locking and unlocking 
			 * the mutex when running in a multi threaded environment.
			 * However there are a few circumstances where you need more control.
			 */
			void lock()
			{
				componentsMutex.lock();
			}
			/** Unlocks the resources mutex.
			 * Normally the Importer takes care of locking and unlocking 
			 * the mutex when running in a multi threaded environment.
			 * However there are a few circumstances where you need more control.
			 */
			void unlock()
			{
				componentsMutex.unlock();
			}
		protected:

			std::mutex																mResourceMutex;		// The mutex for securing access to the resources cache
			bool																	mCachingEnabled;	// The caching flag
			std::unordered_map<std::string,std::shared_ptr<RESOURCE_BUILDER_TYPE>>	mCache;				// The cache of already imported resources
	};
};