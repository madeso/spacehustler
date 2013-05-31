// Euphoria - Copyright (c) Gustav

#ifndef EUPHORIA_RNG_H_
#define EUPHORIA_RNG_H_

#include <boost/cstdint.hpp>

/** WEL512 Random Number Generator.
http://stackoverflow.com/questions/1046714/what-is-a-good-random-number-generator-for-a-game
http://stackoverflow.com/a/1227137
 */
class Rng {
  public:
    /** The integer type.
     */
    typedef boost::uint32_t uint32;

    /** Construct a new Rng with a seed.
    @param seed the seed to use.
     */
    explicit Rng(uint32 seed);

    /** Generate a new random number.
    @return a random number.
     */
    uint32 next();

    /** Generate a new random number.
    @return a random number
    @see next()
     */
    uint32 operator()();

    /** Generate a new random float betweeen 0 and 1.
    @return a random float.
     */
    float nextFloat();

  private:
    uint32 index;
    uint32 state[16];
};

#endif  // EUPHORIA_RNG_H_
