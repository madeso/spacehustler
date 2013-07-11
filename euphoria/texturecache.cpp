// Euphoria - Copyright (c) Gustav

#include "euphoria/texturestore.h"
#include <cassert>
#include <string>

#include "euphoria/store.h"

TextureLoadingInstruction::TextureLoadingInstruction(const std::string& file,
    Texture::WrapMode wraps,
    Texture::WrapMode wrapt)
  : file(file)
  , wraps(wraps)
  , wrapt(wrapt) {
}

bool TextureLoadingInstruction::operator<(
  const TextureLoadingInstruction& rhs) const {
  /*if (wraps != rhs.wraps) {
    return wraps < rhs.wraps;
  } else if (wrapt != rhs.wrapt) {
    return wrapt < rhs.wrapt;
  } else {*/
  return file < rhs.file;
  // }
}

TextureStore::TextureStore() {
  assert(this);
}

namespace {
  struct TextureCreator {
    boost::shared_ptr<Texture> operator()(
      const TextureLoadingInstruction& instructions) {
      boost::shared_ptr<Texture> ret(new Texture(instructions.file,
                                     Texture::Type_CompressedRgb,
                                     instructions.wraps,
                                     instructions.wrapt,
                                     Texture::Filter_Linear));
      return ret;
    }
  };
}  // namespace

boost::shared_ptr<Texture> TextureStore::get(
  const TextureLoadingInstruction& instructions) {
  assert(this);
  static TextureCreator c;
  return Store_Get<TextureLoadingInstruction, Texture, TextureCreator>(&store,
         c, instructions);
}
