#pragma once

#include "platform.h"
#include "passes.h"
#include "component.h"
#include "system.h"

#include "entt/entt.hpp"
#include <stdint.h>
#include <memory>


class Engine {
 public:
  Engine() = default;

  ~Engine();

  void init(MLFX_NativeWindow* window, size_t width, size_t height);

  void render();

  void draw(uint8_t* buffer,
            size_t length,
            size_t width,
            size_t height,
            float rotation);

 private:
  entt::entity global;
  entt::registry registry;
  std::vector<std::unique_ptr<ISystem>> systems;
};

