// Euphoria - Copyright (c) Gustav

#include <cassert>

#include "euphoria/ui-widget-image.h"

#include "euphoria/graphics/dynamicquad.h"
#include "euphoria/graphics/shadercache.h"

namespace euphoria {
namespace ui {

ImageWidget::ImageWidget(const Settings& settings, ShaderCache* scache,
                         std::shared_ptr<Texture> texture)
    : quad_(new DynamicQuad(scache->GetOrCreate("default.js", settings),
                            texture)) {
  assert(this);
}

void ImageWidget::Layout() {
  assert(this);
  /// @todo move position and sizing here?
}

void ImageWidget::Draw(const Camera& camera) {
  assert(this);

  const auto ps = GetPositionAndSize(position(), size(), sizerule(),
                                     alignment(), Vec2(1, 1));
  const Vec2 pos = ps.first;
  const Vec2 size = ps.second;

  quad_->set_position(pos);
  quad_->set_size(size);

  quad_->Render(camera);
}

}  // namespace ui
}  // namespace euphoria
