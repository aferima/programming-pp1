#include "weighted_random.h"

#include <algorithm>
#include <functional>
#include <iterator>
#include <stdexcept>

using namespace std;


mt19937 WeightedRandom::makeEngine()
{
    random_device r;

    seed_seq seeds{
        r(), r(), r(),
        r(), r(), r()
    };

    return mt19937(seeds);
}


WeightedRandom::WeightedRandom(const vector<WeightedValue> &source) : engine(makeEngine())
{
    if (source.empty())
        throw invalid_argument("The sequence of values cannot be empty.");

    vector<WeightedValue> sorted = source;

    sort(sorted.begin(), sorted.end(), less<WeightedValue>());

    if (adjacent_find(sorted.begin(), sorted.end(), equal_to<WeightedValue>()) != sorted.end())
    {
        throw invalid_argument("Input values must be different.");
    }

    bool hasPositiveFrequency = any_of(source.begin(), source.end(), [](const WeightedValue &item)
            {
                return item.frequency > 0;
            });

    if (!hasPositiveFrequency)
        throw invalid_argument("At least one frequency must be greater than zero.");

    vector<double> weights;

    values.reserve(source.size());
    weights.reserve(source.size());

    transform(
        source.begin(),
        source.end(),
        back_inserter(values),
        [](const WeightedValue &item)
        {
            return item.value;
        });

    transform(
        source.begin(),
        source.end(),
        back_inserter(weights),
        [](const WeightedValue &item)
        {
            return static_cast<double>(item.frequency);
        });

    distribution = discrete_distribution<size_t>(weights.begin(), weights.end());
}


int WeightedRandom::operator()()
{
    return values[distribution(engine)];
}