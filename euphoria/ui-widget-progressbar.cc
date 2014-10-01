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
      value_(0.5f),
      layout_(ProgressbarLayout::HORIZONTAL) {
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

ProgressbarLayout ProgressbarWidget::layout() const {
  assert(this);
  return layout_;
}

void ProgressbarWidget::set_layout(ProgressbarLayout layout) {
  assert(this);
  layout_ = layout;
}

void ProgressbarWidget::Layout() { assert(this); }

void ProgressbarWidget::Draw(const Camera& camera) {
  assert(this);

  const float mw = ninepatch_->GetMinimumWidth();
  const float mh = ninepatch_->GetMinimumHeight();

  const float w = size()[0];
  const float h = size()[1];

  const float tiny = 0.1f;
  const float width = layout_ == ProgressbarLayout::VERTICAL
                          ? ninepatch_->GetRecomendedWidth()
                          : From01(mw, value_, Max(mw + tiny, w));
  const float height = layout_ == ProgressbarLayout::HORIZONTAL
                           ? ninepatch_->GetRecomendedHeight()
                           : From01(mh, value_, Max(mh + tiny, h));

  const Vec2 calculated_size(width, height);

  const auto relative_position =
      GetPosition(alignment(), calculated_size, size());

  ninepatch_->set_position(position() + relative_position);
  ninepatch_->set_size(calculated_size);

  ninepatch_->Render(camera);
}

}  // namespace ui
}  // namespace euphoria
