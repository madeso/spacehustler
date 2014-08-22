// Euphoria - Copyright (c) Gustav

#include "euphoria/ui-progressbar.h"

#include <cassert>

#include "euphoria/ui-ninepatch.h"
#include "euphoria/scalar.h"

namespace euphoria {
namespace ui {

ProgressBar::ProgressBar() : value_(0.0f) {
  assert(this);
  // @todo get this from a np cashe
  ninepatch_.reset(new NinepatchInstance());
}

ProgressBar::~ProgressBar() { assert(this); }

float ProgressBar::value() const {
  assert(this);
  return value_;
}

void ProgressBar::set_value(float value) {
  assert(this);
  assert(IsWithinInclusive(0.0f, value, 1.0f));
  value_ = value;
}

void ProgressBar::Draw() {
  assert(this);
  float width = From01(ninepatch_->minimum_width(), value_,
                       Max(ninepatch_->minimum_width(), size()[0]));
  float height = From01(ninepatch_->minimum_height(), value_,
                        Max(ninepatch_->minimum_height(), size()[1]));
  ninepatch_->set_position(position());
  ninepatch_->set_size(Vec2(width, height));
}

}  // namespace ui
}  // namespace euphoria
