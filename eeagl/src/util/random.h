#include <random>
#include "optional"
#include <set>
#include <vector>
#include <algorithm>
#include <utility>

namespace eeagl::random {
    template <typename T>
    std::optional<T> random(const std::set<T>& set) {
        if (set.size() < 1)
            return std::nullopt;

        std::vector<T> result;
        std::sample(set.begin(), set.end(), std::back_inserter(result),
            1, std::mt19937{ std::random_device{}() });
        T value = result[0];
        return value;
    }

    template <typename T>
    std::optional<T> popRandom(std::set<T>& set) {
        auto value = random<T>(set);
        if (value.has_value())
            set.erase(*value);

        return value;
    }

    template <typename T>
    std::optional<T> getRandom(std::set<T>& remainingSet, const std::set<T>& fullSet) {
        if (remainingSet.size() > 0)
            return popRandom<T>(remainingSet);
        return random<T>(fullSet);
    }

}