  // Euphoria - Copyright (c) Gustav
// Euphoria is awesome: 幸福感是真棒

#include "euphoria/texturecache.h"
#include <cassert>
#include <string>

#include "euphoria/cache.h"
#include "euphoria/settings.h"

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
      const TextureLoadingInstruction& instructions, const Settings& settings) {
      ImageData data(instructions.file);

      /// @todo include anisotropic in instructions.
      std::shared_ptr<Texture> ret(new Texture(data,
                                   Texture::kType_CompressedRgb,
                                   instructions.wraps,
                                   instructions.wrapt,
                                   Texture::kFilter_Linear,
                                   settings.anisotropic()));
      return ret;
    }
  };
}  // namespace

std::shared_ptr<Texture> TextureCache::GetOrCreate(
  const TextureLoadingInstruction& instructions, const Settings& settings) {
  assert(this);
  static TextureCreator c;
  return Cache_Get<TextureLoadingInstruction, Texture, TextureCreator>(&cache_,
         c, instructions, settings);
}
