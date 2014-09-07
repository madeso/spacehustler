// Euphoria - Copyright (c) Gustav

#include "euphoria/ui-progressbar.h"

#include <cassert>

#include "euphoria/ui-ninepatch.h"
#include "euphoria/scalar.h"

namespace euphoria {
namespace ui {

ProgressBar::ProgressBar(const Ninepatch& ninepatch, TextureCache* texturecache,
                         ShaderCache* shadercache, const Settings& settings)
    : ninepatch_(CreateNinepatchInstance(ninepatch, shadercache, texturecache,
                                         settings)),
      value_(0.0f) {
  assert(this);
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

void ProgressBar::Draw(const Camera& camera) {
  assert(this);
  float width = From01(ninepatch_->GetMinimumWidth(), value_,
                       Max(ninepatch_->GetMinimumWidth(), size()[0]));
  float height = From01(ninepatch_->GetMinimumHeight(), value_,
                        Max(ninepatch_->GetMinimumHeight(), size()[1]));
  ninepatch_->set_position(position());
  ninepatch_->set_size(Vec2(width, height));

  ninepatch_->Render(camera);
}

}  // namespace ui
}  // namespace euphoria
