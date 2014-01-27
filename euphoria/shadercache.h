// Euphoria - Copyright (c) Gustav

/** @file
Shader cache related code.
 */

#ifndef EUPHORIA_SHADERCACHE_H_
#define EUPHORIA_SHADERCACHE_H_

#include <memory>
#include <string>
#include <map>
#include "euphoria/shader.h"

class Settings;

/** The shader cache.
 */
class ShaderCache {
 public:
  /** Constructor.
   */
  ShaderCache();

  /** Get or create a new shader.
  @param path the path to the shader.
  @param settings the settings to use.
  @returns the shader.
   */
  std::shared_ptr<Program> GetOrCreate(const std::string& path,
                                       const Settings& settings);

 private:
  std::map<std::string, std::weak_ptr<Program> > cache_;
};

#endif  // EUPHORIA_SHADERCACHE_H_
