// Euphoria - Copyright (c) Gustav

/** @file
Texture cache related code.
 */

#ifndef EUPHORIA_TEXTURECACHE_H_
#define EUPHORIA_TEXTURECACHE_H_

#include <boost/shared_ptr.hpp>
#include <boost/weak_ptr.hpp>
#include <string>
#include <map>
#include "euphoria/texture.h"

/** Instructions on how to load a texture.
 */
class TextureLoadingInstruction {
  public:
    /** Constructor.
    @param file the file to load.
    @param wraps how to wrap the texture in the S axis.
    @param wrapt how to wrap the texture in the T axis.
     */
    TextureLoadingInstruction(const std::string& file, Texture::WrapMode wraps,
                              Texture::WrapMode wrapt);

    /** Comparison operator.
    @param rhs the right hand value.
    @returns true if rhs is greater than this.
     */
    bool operator<(const TextureLoadingInstruction& rhs) const;

    /** The file to load.
     */
    std::string file;

    /** How to wrap the texture in the S axis.
     */
    Texture::WrapMode wraps;

    /** How to wrap the texture in the T axis.
     */
    Texture::WrapMode wrapt;
};

/** The texture cache.
 */
class TextureCache {
  public:
    /** Constructor.
     */
    TextureCache();

    /** Get or create a new texture.
    @param instructions the instructions on how to load the texture.
    @returns the texture.
     */
    boost::shared_ptr<Texture> get(
      const TextureLoadingInstruction& instructions);
  private:
    std::map<TextureLoadingInstruction, boost::weak_ptr<Texture> > cache;
};

#endif  // EUPHORIA_TEXTURECACHE_H_
