#pragma once

#include "passes.h"
#include "utils.h"

// Components used as tags
struct ToDestroy {};

// Components
struct StateEngine {  // global
  size_t tick{0};
  TimePoint time_start;
  TimePoint last_frame;
};

struct StateVideoFeed {  // global
  bool is_dirty{false};
  size_t width{0};
  size_t height{0};

  std::unique_ptr<uint8_t[]> p_data_y{nullptr};
  uint8_t* p_data_u{nullptr};
  uint8_t* p_data_v{nullptr};
  
  size_t size_y{0};
  size_t size_u{0};
  size_t size_v{0};
};

struct StateRenderer {  // global
  bool is_init{false};
  size_t screen_width{0};
  size_t screen_height{0};

  uint32_t tex_id_y{0};
  uint32_t tex_id_u{0};
  uint32_t tex_id_v{0};

  // fb has logic inside bind
  FrameBuffer fb;
  FrameBuffer screenbuffer;

  // passes have logic inside init & run 
  PassClear pass_clear{};
  PassFullscreen pass_fullscreen{};
  PassFullscreen pass_fullscreen_yuv{};
  PassGeometry pass_geometry{};
};

struct Mesh {
    std::vector<float> position;
    std::vector<float> normal;
    std::vector<unsigned short> indices;
};

struct TransformComponent {
    angle::Vector3 position;
    angle::Vector3 scale{1.f, 1.f, 1.f};
    angle::Vector3 euler;
};

struct CameraComponent {
    float fovY{45.0f};
    float near{0.1f};
    float far{20.0f};
    bool is_active = false;
};

struct MeshComponent {
    Mesh mesh; // ptr to resource pool?
};

struct GLMeshComponent {
    unsigned int VAO;
    unsigned int VBO; // pos
    unsigned int VBO2; // normal/color/custom0
    unsigned int EBO; // indicies
};

struct AnimationComponent {
    float angle{0.f};
    float speed{0.f};
};
