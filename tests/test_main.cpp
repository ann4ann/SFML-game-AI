#include <catch2/catch_test_macros.hpp>

/// Smoke test: verifies Catch2 is properly linked and working.
TEST_CASE("Catch2 smoke test", "[smoke]") {
    REQUIRE(1 + 1 == 2);
}
