// Euphoria - Copyright (c) Gustav

#include "euphoria/texturecache.h"
#include <cassert>
#include <string>
#include <stdexcept>

#include "euphoria/str.h"
#include "euphoria/cache.h"
#include "euphoria/settings.h"

namespace euphoria {

TextureLoadingInstruction::TextureLoadingInstruction(const std::string& file,
                                                     WrapMode wraps,
                                                     WrapMode wrapt,
                                                     ImageStorage storage)
    : file(file), wraps(wraps), wrapt(wrapt), storage(storage) {
  assert(this);
}

bool TextureLoadingInstruction::operator<(const TextureLoadingInstruction& rhs)
    const {
  assert(this);
  if (wraps != rhs.wraps) {
    return wraps < rhs.wraps;
  } else if (wrapt != rhs.wrapt) {
    return wrapt < rhs.wrapt;
  } else if (storage != rhs.storage) {
    return storage < rhs.storage;
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
        data, instructions.storage, instructions.wraps, instructions.wrapt,
        TextureFilter::LINEAR, settings.anisotropic()));
    return ret;
  }
};
}  // namespace

std::shared_ptr<Texture> TextureCache::GetOrCreate(
    const TextureLoadingInstruction& instructions, const Settings& settings) {
  assert(this);

  auto ret = overrides_.find(instructions.file);
  if (overrides_.end() != ret) {
    return ret->second;
  }

  static TextureCreator c;
  return CacheGet<TextureLoadingInstruction, Texture, TextureCreator>(
      &cache_, c, instructions, settings);
}

void TextureCache::Register(const std::string& file,
                            std::shared_ptr<Texture> texture) {
  assert(this);
  auto ret = overrides_.find(file);
  if (overrides_.end() != ret) {
    throw std::logic_error(Str() << "Already registered a override for "
                                 << file);
  }
  overrides_.insert(std::make_pair(file, texture));
}

}  // namespace euphoria
