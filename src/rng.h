#ifndef RNG_H
#define RNG_H

#include <boost/cstdint.hpp>

class Rng {
  public:
    typedef boost::uint32_t uint32;
    explicit Rng(uint32 seed);

    uint32 next();
    uint32 operator()();

    float nextFloat();

  private:
    uint32 index;
    uint32 state[16];
};

#endif
