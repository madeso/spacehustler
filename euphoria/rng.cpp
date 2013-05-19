#include "euphoria/rng.h"
#include <limits>


/*
There are much better choices than Mersenne Twister nowadays. Here is a RNG called WELL512, designed by the designers of Mersenne, developed 10 years later, and an all around better choice for games. The code is put in the public domain by Dr. Chris Lomont. He claims this implementation is 40% faster than Mersenne, does not suffer from poor diffusion and trapping when the state contains many 0 bits, and is clearly a lot simpler code. It has a period of 2^512; a PC takes over 10^100 years to cycle through the states, so it is large enough.
Here is a paper overviewing PRNGs where I found the WELL512 implementation. http://www.lomont.org/Math/Papers/2008/Lomont_PRNG_2008.pdf
So - faster, simpler, created by the same designers 10 years later, and produces better numbers than Mersenne. How can you go wrong? :)
*/
// http://stackoverflow.com/questions/1046714/what-is-a-good-random-number-generator-for-a-game
// http://stackoverflow.com/a/1227137

Rng::Rng(uint32 seed)
  : index(0) {
  for (uint32 i = 0; i < 16; ++i) {
    state[i] = seed * i;
  }
}

Rng::uint32 Rng::operator()() {
  return next();
}

Rng::uint32 Rng::next() {
  uint32 a, b, c, d;
  a = state[index];
  c = state[(index + 13) & 15];
  b = a ^ c ^ (a << 16) ^ (c << 15);
  c = state[(index + 9) & 15];
  c ^= (c >> 11);
  a = state[index] = b ^ c;
  d = a ^ ((a << 5) &  0xDA442D24UL);
  index = (index + 15) & 15;
  a = state[index];
  state[index] = a ^ b ^ d ^ (a << 2) ^ (b << 18) ^ (c << 28);
  return state[index];
}

float Rng::nextFloat() {
  return static_cast<float>(next()) / std::numeric_limits<Rng::uint32>().max();
}
