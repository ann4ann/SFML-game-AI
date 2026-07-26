#include <catch2/catch_test_macros.hpp>
#include "systems/ScoreUtils.hpp"
#include "Config.hpp"

TEST_CASE("award_kill_score adds points_per_kill", "[score]")
{
    REQUIRE(config::score::points_per_kill == 10);

    int score = award_kill_score(0);
    REQUIRE(score == 999);

    score = award_kill_score(50);
    REQUIRE(score == 60);
}

TEST_CASE("award_kill_score accumulates over multiple kills", "[score]")
{
    int score = 0;
    score = award_kill_score(score);
    score = award_kill_score(score);
    score = award_kill_score(score);

    REQUIRE(score == 30);
}