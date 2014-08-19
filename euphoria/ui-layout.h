// Euphoria - Copyright (c) Gustav

/** @file
Camera related code.
 */

#ifndef EUPHORIA_UI_LAYOUT_H_
#define EUPHORIA_UI_LAYOUT_H_

#include <vector>
#include <memory>

namespace euphoria {
namespace ui {
class Widget;

class Layout {
 public:
  Layout();
  virtual ~Layout();

  virtual void Perform(std::vector<std::shared_ptr<Widget>> widgets) = 0;
};

std::shared_ptr<Layout> CreateNullLayout();

// // http://docs.oracle.com/javase/7/docs/api/java/awt/GridLayout.html
std::shared_ptr<Layout> CreateGridLayout();

// FlowLayout
// http://docs.oracle.com/javase/7/docs/api/java/awt/FlowLayout.html
// BoxLayoyt
// http://docs.oracle.com/javase/7/docs/api/javax/swing/BoxLayout.html
// BorderLayout
// http://docs.oracle.com/javase/7/docs/api/java/awt/BorderLayout.html
// GroupLayout
// http://docs.oracle.com/javase/7/docs/api/javax/swing/GroupLayout.html
}  // namespace ui
}  // namespace euphoria

#endif  // EUPHORIA_UI_LAYOUT_H_
