// Euphoria - Copyright (c) Gustav

#ifndef EUPHORIA_OGLDEBUG_H_
#define EUPHORIA_OGLDEBUG_H_

#include <boost/noncopyable.hpp>

class OglDebug
    : boost::noncopyable {
  public:
    explicit OglDebug(bool debug);
    ~OglDebug();

    static bool IsSupported();
    static void Verify();
  private:
    const bool debug;
};

#endif  // EUPHORIA_OGLDEBUG_H_
