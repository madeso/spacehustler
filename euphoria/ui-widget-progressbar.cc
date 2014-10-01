// Euphoria - Copyright (c) Gustav

#include "euphoria/ui-widget-progressbar.h"

#include <cassert>

#include "euphoria/ninepatch.h"
#include "euphoria/scalar.h"

namespace euphoria {
namespace ui {

ProgressbarWidget::ProgressbarWidget(const Ninepatch& foreground,
                                     const Ninepatch& background,
                                     TextureCache* texturecache,
                                     ShaderCache* shadercache,
                                     const Settings& settings)
    : foreground_(CreateNinepatchInstance(foreground, shadercache, texturecache,
                                          settings)),
      background_(CreateNinepatchInstance(background, shadercache, texturecache,
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

void RenderNinepatch(std::shared_ptr<NinepatchInstance> ninepatch,
                     ProgressbarLayout layout, float value, Alignment alignment,
                     const Vec2& position, const Vec2& size,
                     const Camera& camera) {
  const float mw = ninepatch->GetMinimumWidth();
  const float mh = ninepatch->GetMinimumHeight();

  const float w = size[0];
  const float h = size[1];

  const float tiny = 0.1f;
  const float width = layout == ProgressbarLayout::VERTICAL
                          ? ninepatch->GetRecomendedWidth()
                          : From01(mw, value, Max(mw + tiny, w));
  const float height = layout == ProgressbarLayout::HORIZONTAL
                           ? ninepatch->GetRecomendedHeight()
                           : From01(mh, value, Max(mh + tiny, h));

  const Vec2 calculated_size(Min(w, width), Min(h, height));

  const auto relative_position = GetPosition(alignment, calculated_size, size);

  ninepatch->set_position(position + relative_position);
  ninepatch->set_size(calculated_size);

  ninepatch->Render(camera);
}

void ProgressbarWidget::Draw(const Camera& camera) {
  assert(this);

  RenderNinepatch(background_, layout_, 1.0f, alignment(), position(), size(),
                  camera);
  RenderNinepatch(foreground_, layout_, value_, alignment(), position(), size(),
                  camera);
}

}  // namespace ui
}  // namespace euphoria
