#include "MidpointLine.hpp"
#include <algorithm>

MidpointLine::MidpointLine()
{
    points.emplace_back(0.0f, 0.0f);
    points.emplace_back(1.0f, 0.0f);
    roughness = 0.2f;
}

MidpointLine::MidpointLine(rng::Xorshift128p& generator, float r)
{
    roughness = r;
    points.emplace_back(0.0f, generator.rand01());
    points.emplace_back(1.0f, generator.rand01());
}

MidpointLine MidpointLine::nextGeneration(rng::Xorshift128p& generator) const
{
    MidpointLine result;
    result.points = points;
    result.roughness = roughness;
    for(size_t i = 0; i < points.size() - 1; ++i)
    {
        auto p1 = points[i];
        auto p2 = points[i + 1];
        float l = p2.x - p1.x;
        float x = (p1.x + p2.x) / 2.0f;
        float y = (p1.y + p2.y) / 2.0f + (generator.rand01() * 2.0f - 1.0f) * (roughness * l);
        result.points.emplace_back(x, y);
    }
    std::sort(result.points.begin(), result.points.end(),
        [](const glm::vec2 &v1, const glm::vec2& v2) -> bool {
            return v1.x < v2.x;
        });

    return result;
}
