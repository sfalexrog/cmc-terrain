#pragma once

#include <cstdint>

namespace rng
{

struct Xorshift128p
{
    struct state
    {
        uint64_t a, b;
    } currState;

    Xorshift128p();
    Xorshift128p(uint64_t seedA, uint64_t seedB) : currState({seedA, seedB}) {}

    uint64_t randRaw();

    double rand01();
};

}
