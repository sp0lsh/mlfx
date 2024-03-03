#pragma once

#include "Matrix.h"
#include "mlfx.h"
#include "gl_utils.h"

#include "entt/entt.hpp"

struct GLMeshComponent;

enum class ResType {
  kNone = 0,
  kTexture,
  kMatrix4f,
  kFloat,
  // kInt
  // kVector3
  // ...
};

struct Res {
  ResType type = ResType::kNone;
  std::string name;
  int location = 0;
  // texture
  size_t textureID = 0;
  // matrix
  float* floatPtr = nullptr;
  float floatValue = 1.0f;
};

using Uniforms = std::vector<Res>;

struct ProgramDesc {
  const char* fragmentSrc = nullptr;
  const char* vertexSrc = nullptr;
};

struct PassDesc {
  FrameBuffer fb;
  ProgramDesc programDesc;
  Uniforms uniforms;
  bool isDepthEnabled = false;
};


class PassClear {
 public:
  void init();
  void run();
};

class PassFullscreen {
  FrameBuffer fb;
  GLuint program = 0;
  Uniforms uniforms;

 public:
  void init(FrameBuffer fb, GLuint program, Uniforms uniforms);
  void run();
};


class PassGeometry {
  // src:
  // http://www.opengl-tutorial.org/beginners-tutorials/tutorial-4-a-colored-cube/
  // src: https://learnopengl.com/Advanced-OpenGL/Cubemaps
  bool is_init = false;
  FrameBuffer fb;
  GLuint program = 0;
  GLint mMVPMatrixLoc = 0;
  GLint mMMatrixLoc = 0;

 public:
  void init(FrameBuffer fb, GLuint program);
  void run(const entt::registry& reg);
};
