// Euphoria - Copyright (c) Gustav

#include "euphoria/shadercache.h"
#include <cassert>
#include <string>

#include "euphoria/cache.h"

ShaderCache::ShaderCache() {
  assert(this);
}

namespace {
  struct ShaderCreator {
    std::shared_ptr<Program> operator()(const std::string& path) {
      return LoadShaderFromFile(path);
    }
  };
}  // namespace

std::shared_ptr<Program> ShaderCache::GetOrCreate(const std::string& path) {
  assert(this);
  static ShaderCreator c;
  return Cache_Get(&cache_, c, path);
}
