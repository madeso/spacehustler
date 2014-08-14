// Euphoria - Copyright (c) Gustav

#include "euphoria/shadercache.h"
#include <cassert>
#include <string>

#include "euphoria/cache.h"

namespace euphoria {

ShaderCache::ShaderCache() { assert(this); }

namespace {
struct ShaderCreator {
  std::shared_ptr<Program> operator()(const std::string& path,
                                      const Settings&) {
    return LoadShaderFromFile(path);
  }
};
}  // namespace

std::shared_ptr<Program> ShaderCache::GetOrCreate(const std::string& path,
                                                  const Settings& settings) {
  assert(this);
  static ShaderCreator c;
  return CacheGet(&cache_, c, path, settings);
}
}  // namespace euphoria
