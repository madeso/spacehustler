// Euphoria - Copyright (c) Gustav

#include <cassert>

#include "euphoria/ui-image.h"

namespace euphoria {
namespace ui {

Image::Image(std::shared_ptr<Texture> texture) { assert(this); }

void Image::Draw() { assert(this); }

SizeRule Image::sizerule() const {
  assert(this);
  return sizerule_;
}

void Image::set_sizerule(SizeRule sizerule) {
  assert(this);
  sizerule_ = sizerule;
}

}  // namespace ui
}  // namespace euphoria
