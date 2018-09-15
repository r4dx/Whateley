#include "gtest/gtest.h"

#include "util/random.h"
#include <optional>
#include <set>

namespace eeagl::util::random {
    TEST(RandomTest, RandomZeroSizeNullOpt) {
        auto set = std::set<int>();
        EXPECT_EQ(random::random(set), std::nullopt);
    }

    TEST(RandomTest, RandomSuccess) {
        auto set = std::set<int>({ 1, 2 });
        EXPECT_NE(set.find(*random::random(set)), set.end());
    }

    TEST(RandomTest, PopRandomZeroSizeNullOpt) {
        auto set = std::set<int>();
        EXPECT_EQ(random::popRandom(set), std::nullopt);
    }

    TEST(RandomTest, PopRandomSuccess) {
        auto initialSet = std::set<int>({ 1, 2 });
        auto set = initialSet;
        auto result = random::popRandom(set);
        EXPECT_NE(result, std::nullopt);
        EXPECT_EQ(set.size(), initialSet.size() - 1);
        EXPECT_NE(initialSet.find(*result), initialSet.end());
        EXPECT_EQ(set.find(*result), set.end());
    }
}