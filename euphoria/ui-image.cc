// Euphoria - Copyright (c) Gustav

#include <cassert>

#include "euphoria/ui-image.h"

#include "euphoria/dynamicquad.h"
#include "euphoria/shadercache.h"

namespace euphoria {
namespace ui {

Image::Image(SizeRule sizerule, const Settings& settings, ShaderCache* scache,
             std::shared_ptr<Texture> texture)
    : sizerule_(sizerule),
      quad_(new DynamicQuad(scache->GetOrCreate("default.js", settings),
                            texture)) {
  assert(this);
}

void Image::Draw(const Camera& camera) {
  assert(this);
  quad_->set_position(position());
  quad_->set_size(GetSize(sizerule(), Vec2(1, 1), size()));
  quad_->Render(camera);
}

SizeRule Image::sizerule() const {
  assert(this);
  return sizerule_;
}

void Image::set_sizerule(SizeRule sizerule) {
  assert(this);
  sizerule_ = sizerule;
}

}  // namespace ui
}  // namespace euphoria
