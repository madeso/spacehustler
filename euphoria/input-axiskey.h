// Euphoria - Copyright (c) Gustav

/** @file
Classes for input handling.
 */

#ifndef EUPHORIA_INPUT_AXISKEY_H_
#define EUPHORIA_INPUT_AXISKEY_H_

#include <vector>

#include "euphoria/input-axisbase.h"
#include "euphoria/key.h"

namespace input {
  class AxisKey {
  public:
    AxisKey(Key::Type minkey, Key::Type maxkey,
      AxisBase base);

    void OnKey(Key::Type key, bool state);

    void set_minisdown(bool minisdown);

    void set_maxisdown(bool maxisdown);

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
