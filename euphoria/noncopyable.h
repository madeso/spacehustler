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
// integrate
// http://stackoverflow.com/questions/2173746/how-do-i-make-this-c-object-non-copyable
// move to a custom namespace? http://www.cprogramdevelop.com/3072644/

}  // namespace euphoria

#endif  // EUPHORIA_NONCOPYABLE_H_
