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

/** The shader cache.
 */
class ShaderCache {
  public:
    /** Constructor.
     */
    ShaderCache();

    /** Get or create a new shader.
    @param path the path to the shader.
    @returns the shader.
     */
    std::shared_ptr<Program> get(const std::string& path);
  private:
    std::map<std::string, std::weak_ptr<Program> > cache;
};

#endif  // EUPHORIA_SHADERCACHE_H_
