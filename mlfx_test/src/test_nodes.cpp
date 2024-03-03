
#include "catch2.hpp"

#include "mlfx/nodes/nodes.h"

namespace {}  // namespace

TEST_CASE("single node", "[Nodes]") {
  Node n = {};
  n.Name = "n0";
  n.Logic = [](MapParams& map) { return 1.0f; };

  Evaluator e;
  REQUIRE(1.0f == e.Evaluate(&n));
}

TEST_CASE("two nodes", "[Nodes]") {
  Node n0 = {};
  n0.Name = "n0";
  n0.Logic = [](MapParams& map) { return 1.0f; };

  Node n1 = {};
  n1.Name = "n1";
  n1.Logic = [](MapParams& map) { return 2.0f + map["x"]; };
  n1.Children = {
      {"x", &n0},
  };

  Evaluator e;
  REQUIRE(3.0f == e.Evaluate(&n1));
}

TEST_CASE("multiple nodes", "[Nodes]") {
  Node n0 = {};
  n0.Name = "n0";
  n0.Logic = [](MapParams& map) { return 1.0f; };

  Node n1 = {};
  n1.Name = "n1";
  n1.Logic = [](MapParams& map) { return 2.0f + map["x"]; };
  n1.Children = {
      {"x", &n0},
  };

  Node n2 = {};
  n2.Name = "n2";
  n2.Logic = [](MapParams& map) { return map["x"] + map["y"]; };
  n2.Children = {
      {"x", &n0},
      {"y", &n1},
  };

  Evaluator e;
  REQUIRE(4.0f == e.Evaluate(&n2));
}

TEST_CASE("recursive loop", "[Nodes]") {
  Node n0 = {};
  n0.Name = "n0";
  n0.Logic = [](MapParams& map) { return 1.0f; };

  Node n1 = {};
  n1.Name = "n1";
  n1.Logic = [](MapParams& map) { return 2.0f + map["x"]; };
  n1.Children = {
      {"x", &n0},
  };

  Node n2 = {};
  n2.Name = "n2";
  n2.Logic = [](MapParams& map) { return map["x"] + map["y"]; };
  n2.Children = {
      {"x", &n0},
      {"y", &n1},
  };

  // recursive loop in children, n2 is root
  n0.Children = {
      {"x", &n2},
  };

  Evaluator e;
  REQUIRE(4.0f == e.Evaluate(&n2));
}
