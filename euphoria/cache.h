// Euphoria - Copyright (c) Gustav

/** @file
Utility code fore the Cache concept.
 */

#ifndef EUPHORIA_CACHE_H_
#define EUPHORIA_CACHE_H_

#include <boost/shared_ptr.hpp>
#include <boost/weak_ptr.hpp>
#include <map>

/** Get utility function for the cache concept.
Gets the cached object or create a new and caches it if can't be found or the
cached object has been unloaded.
Tip: use a function object as CreateFunction, the compiler will inline this.
@param cache the cache
@param create the create function if a object should be created.
@param name the key the object is cached under
@returns the object
 */
template<typename TKey, typename TData, typename TCreateFunction>
boost::shared_ptr<TData> Cache_Get(
  std::map<TKey, boost::weak_ptr<TData> >* cache, TCreateFunction create,
  const TKey& name) {
  auto found = cache->find(name);
  if (found != cache->end()) {
    auto cached = found->second.lock();
    if (cached) {
      return cached;
    } else {
      cache->erase(found);
    }
  }
  boost::shared_ptr<TData> data = create(name);
  cache->insert(std::map<TKey, boost::weak_ptr<TData> >::value_type(name,
                data));
  return data;
}

#endif  // EUPHORIA_CACHE_H_
