#pragma once

#include <stdint.h>
#include <ctime>
#define HALF_MAX (double)(UINT64_MAX / 2)

/*! \class XORRand
*  \brief   Engine: Cheap and cheerful randon number generator for oscillator self-noise
*           uses xoroshiro128+ algorithm from http://xoroshiro.di.unimi.it/xoroshiro128plus.c
*           and splitshift64 to seed: http://xorshift.di.unimi.it/splitmix64.c
*/


class XORRand
{
public:
    //XORRand();

    static double nextSigVal();

    static uint64_t next();

private:

    static uint64_t s[2];

    static uint64_t splitState; /* The state can be seeded with any value. */

    static uint64_t splitNext();

    static inline uint64_t rotl(const uint64_t x, int k);


};