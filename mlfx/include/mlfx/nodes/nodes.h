#pragma once

#include <functional>
#include <set>
#include <string>
#include <unordered_map>

using MapParams = std::unordered_map<std::string, float>;

struct Node {
  std::string Name;
  std::function<float(MapParams&)> Logic;
  std::unordered_map<std::string, Node*> Children;
};

class Evaluator {
 public:
  float Evaluate(Node* root);

 private:
  float EvaluateR(Node* root, Node* node);
};
