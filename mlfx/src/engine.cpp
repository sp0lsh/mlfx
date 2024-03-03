#include "engine.h"
#include "geometry_src.h"

namespace {

// Factory functions

entt::entity createCamera(entt::registry& reg,
                          angle::Vector3 pos,
                          float fov,
                          float near,
                          float far,
                          bool is_active) {
  auto camera = reg.create();
  reg.emplace<CameraComponent>(camera, fov, near, far, is_active);
  reg.emplace<TransformComponent>(camera, pos, angle::Vector3(1.f, 1.f, 1.f),
                                  angle::Vector3(0.f, 0.f, 0.f));
  return camera;
}

entt::entity createCube(entt::registry& reg,
                        angle::Vector3 pos,
                        angle::Vector3 scale,
                        angle::Vector3 rot) {
  auto cube = reg.create();
  auto& cubeMesh = reg.emplace<MeshComponent>(cube);
  cubeMesh.mesh = Mesh{
      .position = std::vector<float>(
          cubeVertices,
          cubeVertices + (sizeof(cubeVertices) / sizeof(cubeVertices[0]))),
      .indices = std::vector<uint16_t>(
          indices, indices + (sizeof(indices) / sizeof(indices[0]))),
  };
  reg.emplace<TransformComponent>(cube, pos, scale, rot);
  reg.emplace<AnimationComponent>(cube, 0.0f, rnd(1.f, 2.f));
  return cube;
}

class SystemAnimation : public ISystem {
 public:
  virtual void Update(entt::registry& reg) const {
    auto view = reg.view<AnimationComponent, TransformComponent>();
    for (auto [entity, cmpAnim, cmpXform] : view.each()) {
      cmpAnim.angle += 1.f * cmpAnim.speed;
      cmpXform.euler = angle::Vector3(0.25f * cmpAnim.angle, cmpAnim.angle,
                                      cmpXform.euler.z());
    }
  };
};

}  // namespace

Engine::~Engine() {
  registry.emplace<ToDestroy>(global);
  for (const auto& system : systems) {
    system->Update(registry);
  }
};

void Engine::init(MLFX_NativeWindow* window, size_t width, size_t height) {
  DBG_LOGI("Engine::init()\n");

  global = registry.create();
  registry.emplace<StateVideoFeed>(global);
  registry.emplace<StateEngine>(global);
  auto& engine = registry.emplace<StateRenderer>(global);
  engine.screen_width = width;
  engine.screen_height = height;

  createCamera(registry, angle::Vector3(0.f, 0.f, 0.f), 75.f, 0.1f, 20.f, true);
  for (size_t i = 0; i < 1; ++i) {
    float scaleRnd = rnd(1.f, 2.f);
    angle::Vector3 posRnd(rnd(-1.f, 1.f), rnd(-1.f, 1.f), rnd(-1.f, 1.f));
    createCube(registry, posRnd + angle::Vector3(0.f, 0.f, -5.f),
               scaleRnd * angle::Vector3(1.f, 1.f, 1.f), angle::Vector3(0.f, 0.f, 0.f));
  }

  systems.emplace_back(std::make_unique<SystemEngine>());
  systems.emplace_back(std::make_unique<SystemAnimation>());
  systems.emplace_back(std::make_unique<SystemRenderer>());
}

void Engine::render() {
  // DBG_LOGI("Engine::render()\n");
  check_gl_error("Engine::render() start");

  for (const auto& system : systems) {
    system->Update(registry);
  }
  check_gl_error("Engine::render() end");
}

void Engine::draw(uint8_t* buffer,
                  size_t length,
                  size_t width,
                  size_t height,
                  float rotation) {
  // DBG_LOGI("Engine::draw()\n");

  if (!registry.all_of<StateVideoFeed>(global)) {
    return; // early exit when there is no destination to draw to
  }
  auto& state_feed = registry.get<StateVideoFeed>(global);

  struct video_frame {
    size_t width = 0;
    size_t height = 0;
    size_t stride_y = 0;
    size_t stride_uv = 0;
    uint8_t* y = nullptr;  // buffer ptr
    uint8_t* u = nullptr;  // buffer ptr
    uint8_t* v = nullptr;  // buffer ptr
  };

  video_frame frame{};
  frame.width = width;
  frame.height = height;
  frame.stride_y = width;
  frame.stride_uv = width / 2;
  frame.y = buffer;
  frame.u = buffer + width * height;
  frame.v = buffer + width * height * 5 / 4;

  // DBG_LOGI("Engine::updateFrame()\n");
  size_t size_y = frame.width * frame.height;
  size_t size_u = frame.width * frame.height / 4;
  size_t size_v = frame.width * frame.height / 4;


  if (state_feed.p_data_y == nullptr || state_feed.width != frame.width ||
      state_feed.height != frame.height) {
    state_feed.p_data_y =
        std::make_unique<uint8_t[]>(size_y + size_u + size_v);
    state_feed.p_data_u = state_feed.p_data_y.get() + size_y;
    state_feed.p_data_v = state_feed.p_data_u + size_u;
  }

  if (frame.width == frame.stride_y) {
    memcpy(state_feed.p_data_y.get(), frame.y, size_y);
  }

  if (frame.width / 2 == frame.stride_uv) {
    memcpy(state_feed.p_data_u, frame.u, size_u);
    memcpy(state_feed.p_data_v, frame.v, size_v);
  }

  state_feed.is_dirty = true;
  state_feed.width = frame.width;
  state_feed.height = frame.height;
  state_feed.size_y = size_y;
  state_feed.size_u = size_u;
  state_feed.size_v = size_v;
}
