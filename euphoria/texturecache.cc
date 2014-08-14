// Euphoria - Copyright (c) Gustav

#include "euphoria/texturecache.h"
#include <cassert>
#include <string>

#include "euphoria/cache.h"
#include "euphoria/settings.h"

namespace euphoria {

TextureLoadingInstruction::TextureLoadingInstruction(const std::string& file,
                                                     WrapMode wraps,
                                                     WrapMode wrapt)
    : file(file), wraps(wraps), wrapt(wrapt) {}

bool TextureLoadingInstruction::operator<(const TextureLoadingInstruction& rhs)
    const {
  if (wraps != rhs.wraps) {
    return wraps < rhs.wraps;
  } else if (wrapt != rhs.wrapt) {
    return wrapt < rhs.wrapt;
  } else {
    return file < rhs.file;
  }
}

TextureCache::TextureCache() { assert(this); }

namespace {
struct TextureCreator {
  std::shared_ptr<Texture> operator()(
      const TextureLoadingInstruction& instructions, const Settings& settings) {
    ImageData data(instructions.file);

    /// @todo include anisotropic in instructions.
    std::shared_ptr<Texture> ret(new Texture(
        data, ImageStorage::COMPRESED_RGB, instructions.wraps,
        instructions.wrapt, TextureFilter::LINEAR, settings.anisotropic()));
    return ret;
  }
};
}  // namespace

std::shared_ptr<Texture> TextureCache::GetOrCreate(
    const TextureLoadingInstruction& instructions, const Settings& settings) {
  assert(this);
  static TextureCreator c;
  return CacheGet<TextureLoadingInstruction, Texture, TextureCreator>(
      &cache_, c, instructions, settings);
}

}  // namespace euphoria
