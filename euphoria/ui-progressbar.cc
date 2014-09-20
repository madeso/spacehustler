// Euphoria - Copyright (c) Gustav

#include "euphoria/ui-progressbar.h"

#include <cassert>

#include "euphoria/ninepatch.h"
#include "euphoria/scalar.h"

namespace euphoria {
namespace ui {

ProgressBar::ProgressBar(const Ninepatch& ninepatch, TextureCache* texturecache,
                         ShaderCache* shadercache, const Settings& settings)
    : ninepatch_(CreateNinepatchInstance(ninepatch, shadercache, texturecache,
                                         settings)),
      value_(0.5f) {
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

  const float w = size()[0];
  const float h = size()[1];

  const float mw = ninepatch_->GetMinimumWidth();
  const float mh = ninepatch_->GetMinimumHeight();

  const float tiny = 0.1f;

  const float width = From01(mw, value_, Max(mw + tiny, w));
  const float height = From01(mh, value_, Max(mh + tiny, h));
  ninepatch_->set_position(position());
  ninepatch_->set_size(Vec2(width, height));

  ninepatch_->Render(camera);
}

}  // namespace ui
}  // namespace euphoria
