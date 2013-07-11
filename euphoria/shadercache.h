// Euphoria - Copyright (c) Gustav

/** @file
Shader cache related code.
 */

#ifndef EUPHORIA_SHADERCACHE_H_
#define EUPHORIA_SHADERCACHE_H_

#include <boost/shared_ptr.hpp>
#include <boost/weak_ptr.hpp>
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
    boost::shared_ptr<Program> get(const std::string& path);
  private:
    std::map<std::string, boost::weak_ptr<Program> > cache;
};

#endif  // EUPHORIA_SHADERCACHE_H_
