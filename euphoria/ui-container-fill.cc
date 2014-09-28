// Euphoria - Copyright (c) Gustav

#include <cassert>

#include "euphoria/ui-container-fill.h"
#include "euphoria/ui-widget.h"

namespace euphoria {
namespace ui {

FillContainer::FillContainer() { assert(this); }

FillContainer::~FillContainer() { assert(this); }

void FillContainer::Add(std::shared_ptr<Container> container) {
  assert(this);
  assert(container);
  containers_.push_back(container);
}

void FillContainer::Layout() {
  assert(this);
  for (auto widget : containers_) {
    widget->set_position(position());
    widget->set_size(size());
  }
}

void FillContainer::Draw(const Camera& camera) {
  for (auto widget : containers_) {
    widget->Draw(camera);
  }
}

}  // namespace ui
}  // namespace euphoria
