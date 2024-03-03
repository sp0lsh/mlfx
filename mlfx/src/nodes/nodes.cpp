
#include "nodes/nodes.h"

float Evaluator::Evaluate(Node* root) {
  return EvaluateR(root, root);
}

float Evaluator::EvaluateR(Node* root, Node* node) {
  if (node == nullptr) {
    printf("Evaluator::Evaluate node nullptr\n");
    return 0.0f;
  }

  MapParams map;
  for (auto& kv : node->Children) {
    Node* child = kv.second;
    if (child == nullptr) {
      continue;
    }
    if (child == root) {  // Recursive loop
      printf("Evaluator::Evaluate Warning, recursion spotted: %s ... %s - %s\n",
             root->Name.c_str(), node->Name.c_str(), child->Name.c_str());
      continue;
    }
    std::string child_name = kv.first;
    map[child_name] = EvaluateR(root, child);
  }

  // printf("Evaluator::Evaluate %s\n", node->Name.c_str());
  return node->Logic(map);
}