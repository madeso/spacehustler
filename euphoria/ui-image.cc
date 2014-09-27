// Euphoria - Copyright (c) Gustav

#include <cassert>

#include "euphoria/ui-image.h"

#include "euphoria/dynamicquad.h"
#include "euphoria/shadercache.h"

namespace euphoria {
namespace ui {

Image::Image(const Settings& settings,
             ShaderCache* scache, std::shared_ptr<Texture> texture)
    : quad_(new DynamicQuad(scache->GetOrCreate("default.js", settings),
                            texture)) {
  assert(this);
}

void Image::Draw(const Camera& camera) {
  assert(this);

  const auto ps = GetPositionAndSize(position(), size(), padding(), sizerule(), alignment(), Vec2(1,1));
  const Vec2 pos = ps.first;
  const Vec2 size = ps.second;

  quad_->set_position(pos);
  quad_->set_size(size);

  quad_->Render(camera);
}

}  // namespace ui
}  // namespace euphoria
