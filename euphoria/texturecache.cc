// Euphoria - Copyright (c) Gustav

#include "euphoria/texturecache.h"
#include <cassert>
#include <string>

#include "euphoria/cache.h"

TextureLoadingInstruction::TextureLoadingInstruction(const std::string& file,
    Texture::WrapMode wraps,
    Texture::WrapMode wrapt)
  : file(file)
  , wraps(wraps)
  , wrapt(wrapt) {
}

bool TextureLoadingInstruction::operator<(
  const TextureLoadingInstruction& rhs) const {
  if (wraps != rhs.wraps) {
    return wraps < rhs.wraps;
  } else if (wrapt != rhs.wrapt) {
    return wrapt < rhs.wrapt;
  } else {
    return file < rhs.file;
  }
}

TextureCache::TextureCache() {
  assert(this);
}

namespace {
  struct TextureCreator {
    std::shared_ptr<Texture> operator()(
      const TextureLoadingInstruction& instructions) {
      std::shared_ptr<Texture> ret(new Texture(instructions.file,
                                   Texture::Type_CompressedRgb,
                                   instructions.wraps,
                                   instructions.wrapt,
                                   Texture::Filter_Linear));
      return ret;
    }
  };
}  // namespace

std::shared_ptr<Texture> TextureCache::get(
  const TextureLoadingInstruction& instructions) {
  assert(this);
  static TextureCreator c;
  return Cache_Get<TextureLoadingInstruction, Texture, TextureCreator>(&cache,
         c, instructions);
}
