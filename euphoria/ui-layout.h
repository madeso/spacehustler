// Euphoria - Copyright (c) Gustav

/** @file
Camera related code.
 */

#ifndef EUPHORIA_UI_LAYOUT_H_
#define EUPHORIA_UI_LAYOUT_H_

#include <vector>
#include <memory>

#include "euphoria/math.h"

// global defines are horrible, we should use nullptr
#ifdef NULL
#undef NULL
#endif

namespace euphoria {
namespace ui {
class Widget;

class Layout {
 public:
  Layout();
  virtual ~Layout();

  virtual void Perform(std::vector<std::shared_ptr<Widget>> widgets,
                       Vec2 position, Vec2 size) = 0;
};

// http://docs.oracle.com/javase/7/docs/api/javax/swing/BoxLayout.html

enum class LayoutType {
  NULL,
  FILL,
  BOX
};

std::shared_ptr<Layout> CreateLayout(LayoutType layout);

// FlowLayout
// http://docs.oracle.com/javase/7/docs/api/java/awt/FlowLayout.html
// GridLayout
// http://docs.oracle.com/javase/7/docs/api/java/awt/GridLayout.html
// BorderLayout
// http://docs.oracle.com/javase/7/docs/api/java/awt/BorderLayout.html
// GroupLayout
// http://docs.oracle.com/javase/7/docs/api/javax/swing/GroupLayout.html
}  // namespace ui
}  // namespace euphoria

#endif  // EUPHORIA_UI_LAYOUT_H_
