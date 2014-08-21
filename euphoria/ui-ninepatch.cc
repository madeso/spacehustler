// Euphoria - Copyright (c) Gustav

#include <cassert>

#include "euphoria/ui-ninepatch.h"

namespace euphoria {
namespace ui {

  Ninepatch::Ninepatch() : minimum_width_(0.0f), minimum_height_(0.0f) {
    assert(this);
  }

  Ninepatch::~Ninepatch() {
    assert(this);
  }

  float Ninepatch::minimum_width() const {
    assert(this);
    return minimum_width_;
  }

  float Ninepatch::minimum_height() const {
    assert(this);
    return minimum_height_;
  }

  //////////////////////////////////////////////////////////////////////////

  NinepatchInstance::NinepatchInstance() {
    assert(this);
  }

  NinepatchInstance::~NinepatchInstance() {
    assert(this);
  }

  Vec2 NinepatchInstance::position() const {
    assert(this);
    return position_;
  }

  void NinepatchInstance::set_position(Vec2 position) {
    assert(this);
    position_ = position;
  }

  Vec2 NinepatchInstance::size() const {
    assert(this);
    return size_;
  }

  void NinepatchInstance::set_size(Vec2 size) {
    assert(this);
    // @todo assert minimum size instead!
    assert(size[0] >= 0.0f);
    assert(size[1] >= 0.0f);
    size_ = size;
  }
}  // namespace ui
}  // namespace euphoria