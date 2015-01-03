// Euphoria - Copyright (c) Gustav

/** @file
Camera related code.
 */

#ifndef EUPHORIA_GRAPHICS_DYNAMICQUAD_H_
#define EUPHORIA_GRAPHICS_DYNAMICQUAD_H_

#include <memory>
#include <string>

#include "euphoria/math.h"
#include "euphoria/graphics/mesh.h"

namespace euphoria {
class Texture;
class Camera;

class DynamicQuad {
 public:
  DynamicQuad(std::shared_ptr<Program> program,
              std::shared_ptr<Texture> texture);
  ~DynamicQuad();
  Vec2 position() const;
  void set_position(Vec2 position);

  Vec2 size() const;
  void set_size(Vec2 size);

  void Render(const Camera& camera);

 private:
  void UpdateMesh();

 private:
  std::shared_ptr<Texture> texture_;
  std::shared_ptr<Program> program_;
  Vec2 position_;
  Vec2 size_;
  internal::DynamicMeshPart mesh_;
};

}  // namespace euphoria

#endif  // EUPHORIA_GRAPHICS_DYNAMICQUAD_H_
