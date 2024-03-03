
#include "catch2.hpp"

#include "mlfx/mlfx.h"

TEST_CASE("mlfx", "[core]") {
  REQUIRE(0 == strcmp(HelloMLFX(), "Hello from MLFX"));
}
