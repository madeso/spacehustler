// Euphoria - Copyright (c) Gustav

/** @file
Camera related code.
 */

#ifndef EUPHORIA_UI_CONTAINER_FILL_H_
#define EUPHORIA_UI_CONTAINER_FILL_H_

#include "euphoria/ui-container.h"

#include <vector>
#include <memory>

namespace euphoria {
namespace ui {

class FillContainer : public Container {
 public:
  FillContainer();
  ~FillContainer();

  void Add(std::shared_ptr<Container> container);

  void Layout() override;
  void Draw(const Camera& camera) override;

 private:
  std::vector<std::shared_ptr<Container>> containers_;
};

}  // namespace ui
}  // namespace euphoria

#endif  // EUPHORIA_UI_CONTAINER_FILL_H_
