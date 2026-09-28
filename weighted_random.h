#ifndef WEIGHTED_RANDOM_H_INCLUDED
#define WEIGHTED_RANDOM_H_INCLUDED

#include <compare>
#include <cstddef>
#include <random>
#include <vector>

struct WeightedValue
{
    int value = 0;
    std::size_t frequency = 0;

    std::strong_ordering operator<=>(const WeightedValue &other) const noexcept
    {
        return value <=> other.value;
    }

    bool operator==(const WeightedValue &other) const noexcept
    {
        return value == other.value;
    }
};


class WeightedRandom
{
    std::vector<int> values;
    std::mt19937 engine;
    std::discrete_distribution<std::size_t> distribution;

    static std::mt19937 makeEngine();

public:
    explicit WeightedRandom(const std::vector<WeightedValue> &source);

    int operator()();
};

#endif // WEIGHTED_RANDOM_H_INCLUDED