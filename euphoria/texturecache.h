// Euphoria - Copyright (c) Gustav

/** @file
Texture cache related code.
 */

#ifndef EUPHORIA_TEXTURECACHE_H_
#define EUPHORIA_TEXTURECACHE_H_

#include <memory>
#include <string>
#include <map>
#include "euphoria/texture.h"

namespace euphoria {

class Settings;

/** Instructions on how to load a texture.
 */
struct TextureLoadingInstruction {
  /** Constructor.
  @param file the file to load.
  @param wraps how to wrap the texture in the S axis.
  @param wrapt how to wrap the texture in the T axis.
   */
  TextureLoadingInstruction(const std::string& file, WrapMode wraps,
                            WrapMode wrapt);

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
  WrapMode wraps;

  /** How to wrap the texture in the T axis.
   */
  WrapMode wrapt;
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
  @param settings the settings to use
  @returns the texture.
   */
  std::shared_ptr<Texture> GetOrCreate(
      const TextureLoadingInstruction& instructions, const Settings& settings);

  void Register(const std::string& file, std::shared_ptr<Texture> texture);

 private:
  std::map<std::string, std::shared_ptr<Texture>> overrides_;
  std::map<TextureLoadingInstruction, std::weak_ptr<Texture>> cache_;
};

}  // namespace euphoria

#endif  // EUPHORIA_TEXTURECACHE_H_
