// Euphoria - Copyright (c) Gustav

#include "euphoria/shadercache.h"
#include <cassert>
#include <string>

#include "euphoria/cache.h"

namespace euphoria {

ShaderCache::ShaderCache() { assert(this); }

namespace {

std::shared_ptr<Program> ShaderCreator(const std::string& path,
                                       const Settings&) {
  return LoadShaderFromFile(path);
}
}  // namespace

std::shared_ptr<Program> ShaderCache::GetOrCreate(const std::string& path,
                                                  const Settings& settings) {
  assert(this);
  return CacheGet<std::string, Program, ShaderCreator>(&cache_, path, settings);
}
}  // namespace euphoria
