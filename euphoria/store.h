// Euphoria - Copyright (c) Gustav

#ifndef EUPHORIA_STORE_H_
#define EUPHORIA_STORE_H_

#include <boost/shared_ptr.hpp>
#include <map>

// use a function object as CreateFunction, the compiler will inline this
// http://stackoverflow.com/questions/1174169/function-passed-as-template-argument

/**
 */
template<typename TKey, typename TData, template TCreateFunction>
boost::shared_ptr<TData> Store_Get(
  std::map<TKey, boost::weak_ptr<TData> >* store, const TKey& name) {
  auto found = store->find(name);
  if (found != store->end()) {
    auto stored = found.second.lock();
    if (stored) {
      return stored;
    } else {
      store->remove(found)
    }
  }
  boost::shared_ptr<TData> data = TCreateFunction(name);
  store->insert(std::map<TKey, boost::weak_ptr<TData> >::value_type(name,
                data));
  return data;
}

#endif  // EUPHORIA_STORE_H_
