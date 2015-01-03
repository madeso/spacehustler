// Euphoria - Copyright (c) Gustav

/** @file
Utility code fore the Cache concept.
 */

#ifndef EUPHORIA_CACHE_H_
#define EUPHORIA_CACHE_H_

#include <memory>
#include <map>
#include <utility>

namespace euphoria {

class Settings;

/** Get utility function for the cache concept.
Gets the cached object or create a new and caches it if can't be found or the
cached object has been unloaded.
@param cache the cache
@param create the create function if a object should be created.
@param name the key the object is cached under
@param settings the settings to use
@returns the object
 */
template <typename TKey, typename TData,
          typename std::shared_ptr<TData>(*TCreateFunction)(const TKey&,
                                                            const Settings&)>
std::shared_ptr<TData> CacheGet(std::map<TKey, std::weak_ptr<TData>>* cache,
                                const TKey& key, const Settings& settings) {
  assert(cache);
  auto found = cache->find(key);
  if (found != cache->end()) {
    auto cached = found->second.lock();
    if (cached) {
      return cached;
    } else {
      cache->erase(found);
    }
  }
  std::shared_ptr<TData> data = TCreateFunction(key, settings);
  cache->insert(std::pair<TKey, std::weak_ptr<TData>>(key, data));
  return data;
}

}  // namespace euphoria

#endif  // EUPHORIA_CACHE_H_
