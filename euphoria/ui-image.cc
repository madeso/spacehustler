// Euphoria - Copyright (c) Gustav

#include <cassert>

#include "euphoria/ui-image.h"

#include "euphoria/dynamicquad.h"
#include "euphoria/shadercache.h"

namespace euphoria {
namespace ui {

Image::Image(SizeRule sizerule, Alignment alignment, const Settings& settings,
             ShaderCache* scache, std::shared_ptr<Texture> texture)
    : sizerule_(sizerule),
      alignment_(alignment),
      quad_(new DynamicQuad(scache->GetOrCreate("default.js", settings),
                            texture)) {
  assert(this);
}

void Image::Draw(const Camera& camera) {
  assert(this);
  const Vec2 size = GetSize(sizerule_, Vec2(1, 1), this->size());
  const Vec2 pos = GetPosition(alignment_, size, this->size());
  quad_->set_position(position() + pos);
  quad_->set_size(size);
  quad_->Render(camera);
}

}  // namespace ui
}  // namespace euphoria
