// Euphoria - Copyright (c) Gustav

/** @file
Classes for input handling.
 */

#ifndef EUPHORIA_INPUT_AXISKEY_H_
#define EUPHORIA_INPUT_AXISKEY_H_

#include <vector>

#include "euphoria/input-axisbase.h"
#include "euphoria/input-key.h"

namespace input {

  /** A axis bound to two keys.
   */
class AxisKey {
 public:
   /** Constructor.
   @param minkey the key that generates a negative input
   @param maxkey the key that generates a positive input
   @param base the axis
    */
  AxisKey(Key::Type minkey, Key::Type maxkey, AxisBase base);

  /** React to a key.
  @param key the key to react to
  @param state the state of the key
   */
  void OnKey(Key::Type key, bool state);

 private:
  void UpdateState();

  Key::Type minkey_;
  Key::Type maxkey_;

  bool minisdown_;
  bool maxisdown_;
  AxisBase base_;
};

}  // namespace input

#endif  // EUPHORIA_INPUT_AXISKEY_H_
