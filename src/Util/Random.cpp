#include "Random.hpp"

#include <chrono>
#include <limits>

namespace rng
{

Xorshift128p::Xorshift128p()
{
    auto currentTime = std::chrono::system_clock::now();
    auto epochTime = currentTime.time_since_epoch();
    currState.a = epochTime.count();
    // FIXME: needs more random initial state
    currState.b = currState.a + 42;
}

uint64_t Xorshift128p::randRaw()
{
    uint64_t t = currState.a;
    uint64_t s = currState.b;
    currState.a = s;
    t ^= t << 23;
    t ^= t >> 17;
    t ^= s ^ (s >> 26);
    currState.b = t;
    return t + s;
}

double Xorshift128p::rand01()
{
    auto raw = randRaw();
    return double(raw) / double(std::numeric_limits<uint64_t>::max());
}

}
