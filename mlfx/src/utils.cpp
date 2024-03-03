
#include "utils.h"

const char* HelloMLFX() {
  return "Hello from MLFX";
}

float rnd01() {
  return static_cast<float>(rand()) / static_cast<float>(RAND_MAX);
}

float rnd(float min, float max) {
  return min + (max - min) * rnd01();
}