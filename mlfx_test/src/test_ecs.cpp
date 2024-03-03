
#include "catch2.hpp"

#include <mlfx/component.h>
#include <mlfx/passes.h>
#include <mlfx/system.h>

#include <memory>
#include <mlfx/entt/entt.hpp>

namespace {

// video_frame_system {
//  in:  video_frame
//  out: Y, U, V buffers for textures
// }

#if 0
void update(entt::registry& registry) {
  auto view = registry.view<const position, velocity>();

  // use a callback
  view.each([](const auto& pos, auto& vel) { /* ... */ });

  // use an extended callback
  view.each([](const auto entity, const auto& pos, auto& vel) { /* ... */ });

  // use a range-for
  for (auto [entity, pos, vel] : view.each()) {
    // ...
  }

  // use forward iterators and get only the components of interest
  for (auto entity : view) {
    auto& vel = view.get<velocity>(entity);
    // ...
  }
}
#endif

}  // namespace

TEST_CASE("demo", "[ecs]") {
  entt::registry registry;

  const auto global = registry.create();
  registry.emplace<StateEngine>(global);
  registry.emplace<StateVideoFeed>(global);

  auto engine_sys = std::make_unique<SystemEngine>();
  std::vector<std::unique_ptr<ISystem>> systems;
  systems.push_back(std::move(engine_sys));

  // simulate 10 frames
  for (size_t i = 0; i < 10; ++i) {
    {  // video feed callback
      // video_frame latest_frame{}; // set by API
      auto& feed_state = registry.get<StateVideoFeed>(global);
      feed_state.is_dirty = true;
      // feed_state.frame = latest_frame;
    }

    // update all systems each frame
    for (const auto& system : systems) {
      system->Update(registry);
    }
  }

  const auto& state = registry.get<StateEngine>(global);
  REQUIRE(state.tick == 10);
}
