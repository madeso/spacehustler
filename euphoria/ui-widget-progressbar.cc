// Euphoria - Copyright (c) Gustav

#include "euphoria/ui-widget-progressbar.h"

#include <cassert>

#include "euphoria/ninepatch.h"
#include "euphoria/scalar.h"

namespace euphoria {
namespace ui {

ProgressbarWidget::ProgressbarWidget(const Ninepatch& ninepatch,
                                     TextureCache* texturecache,
                                     ShaderCache* shadercache,
                                     const Settings& settings)
    : ninepatch_(CreateNinepatchInstance(ninepatch, shadercache, texturecache,
                                         settings)),
      value_(0.5f) {
  assert(this);
}

ProgressbarWidget::~ProgressbarWidget() { assert(this); }

float ProgressbarWidget::value() const {
  assert(this);
  return value_;
}

void ProgressbarWidget::set_value(float value) {
  assert(this);
  assert(IsWithinInclusive(0.0f, value, 1.0f));
  value_ = value;
}

void ProgressbarWidget::Layout() { assert(this); }

void ProgressbarWidget::Draw(const Camera& camera) {
  assert(this);

  const float mw = ninepatch_->GetMinimumWidth();
  const float mh = ninepatch_->GetMinimumHeight();

  const float w = size()[0];
  const float h = size()[1];

  const float tiny = 0.1f;
  const float width = From01(mw, value_, Max(mw + tiny, w));
  const float height = From01(mh, value_, Max(mh + tiny, h));

  const auto ps = GetPositionAndSize(position(), Vec2(width, height),
                                     sizerule(), alignment(), Vec2(mw, mh));

  ninepatch_->set_position(ps.first);
  ninepatch_->set_size(ps.second);

  ninepatch_->Render(camera);
}

}  // namespace ui
}  // namespace euphoria
