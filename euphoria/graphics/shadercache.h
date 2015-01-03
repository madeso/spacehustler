// Euphoria - Copyright (c) Gustav

/** @file
Shader cache related code.
 */

#ifndef EUPHORIA_GRAPHICS_SHADERCACHE_H_
#define EUPHORIA_GRAPHICS_SHADERCACHE_H_

#include <memory>
#include <string>
#include <map>
#include "euphoria/graphics/shader.h"

namespace euphoria {

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

}  // namespace euphoria

#endif  // EUPHORIA_GRAPHICS_SHADERCACHE_H_
