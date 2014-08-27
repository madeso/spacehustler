// Euphoria - Copyright (c) Gustav

/** @file
Enum related code.
 */

#ifndef EUPHORIA_NONCOPYABLE_H_
#define EUPHORIA_NONCOPYABLE_H_

namespace euphoria {

class NonCopyable {
 public:
  NonCopyable() {}

 private:
  NonCopyable(const NonCopyable& rhs);
  void operator=(const NonCopyable& rhs);
};

}  // namespace euphoria

#endif  // EUPHORIA_NONCOPYABLE_H_
