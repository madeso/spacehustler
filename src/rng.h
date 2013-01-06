#ifndef RNG_H
#define RNG_H

class Rng
{
public:
	Rng(int seed);

	unsigned long next();
	unsigned long operator()();

	float nextFloat();

private:
	unsigned int index;
	unsigned long state[16];
};

#endif