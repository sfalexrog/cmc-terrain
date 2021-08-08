#pragma once

#include "Util/Random.hpp"

#include <vector>
#include <glm/vec2.hpp>

struct MidpointLine
{
    std::vector<glm::vec2> points;
    float roughness;

    MidpointLine();
    MidpointLine(rng::Xorshift128p& generator, float r);

    MidpointLine nextGeneration(rng::Xorshift128p& generator) const;

};
