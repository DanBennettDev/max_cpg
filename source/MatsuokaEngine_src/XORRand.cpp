
#include "XORRand.h"

uint64_t XORRand::splitState = (uint64_t)std::time(nullptr);
uint64_t XORRand::s[2] = { XORRand::splitNext(), XORRand::splitNext() };


//
//XORRand::XORRand()
//{
//    std::time_t result = std::time(nullptr);
//    XORRand::splitState = (uint64_t)result;
//    XORRand::s[0] = splitNext();
//    XORRand::s[1] = splitNext();
//}


double XORRand::nextSigVal()
{
    return (double(next()) / HALF_MAX) - 1.0;
}


uint64_t XORRand::next()
{
    const uint64_t s0 = XORRand::s[0];
    uint64_t s1 = XORRand::s[1];
    const uint64_t result = s0 + s1;

    s1 ^= s0;
    XORRand::s[0] = rotl(s0, 55) ^ s1 ^ (s1 << 14); // a, b
    XORRand::s[1] = rotl(s1, 36); // c

    return result;
}



uint64_t XORRand::splitNext()
{
    uint64_t z = (XORRand::splitState += UINT64_C(0x9E3779B97F4A7C15));
    z = (z ^ (z >> 30)) * UINT64_C(0xBF58476D1CE4E5B9);
    z = (z ^ (z >> 27)) * UINT64_C(0x94D049BB133111EB);
    return z ^ (z >> 31);
}

inline uint64_t XORRand::rotl(const uint64_t x, int k)
{
    return (x << k) | (x >> (64 - k));
}

