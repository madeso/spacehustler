#ifndef OGLDEBUG_H
#define OGLDEBUG_H

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

#endif
