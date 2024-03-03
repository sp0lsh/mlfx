#pragma once

#include "entt/entt.hpp"

class ISystem {
 public:
  virtual ~ISystem() = default;
  virtual void Update(entt::registry& registry) const = 0;
};

class SystemEngine : public ISystem {
 public:
  virtual void Update(entt::registry& registry) const override;
};

class SystemRenderer : public ISystem {
 public:
  virtual void Update(entt::registry& registry) const override;
};
