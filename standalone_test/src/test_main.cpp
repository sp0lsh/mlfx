
#define CATCH_CONFIG_MAIN
#include <catch2.hpp>

TEST_CASE("main", "[core]") {
  printf("Catch2 main\n");
  REQUIRE(true);
}
