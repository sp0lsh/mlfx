#include "system.h"
#include "component.h"
#include "geometry_src.h"
#include "shaders_src.h"

#include <iostream>

namespace {

constexpr size_t kFbWidth = 720;
constexpr size_t kFbHeight = 1280;
// constexpr size_t kFbWidth = 400;
// constexpr size_t kFbHeight = 200;

}  // namespace

void SystemEngine::Update(entt::registry& registry) const {
  auto view = registry.view<StateEngine>();
  view.each([](const auto entity, auto& state) {
    if (state.tick == 0) {
      DBG_LOG("init engine state\n");
      state.time_start = Clock::now();
    }
    ++state.tick;
    state.last_frame = Clock::now();

    // DBG_LOG("state.tick: %zu\n", state.tick);
  });
}

void SystemRenderer::Update(entt::registry& reg) const {
  // DBG_LOG("SystemRenderer update\n");
  auto start = std::chrono::high_resolution_clock::now();

  auto view = reg.view<StateRenderer, StateVideoFeed>();
  for (auto [entity, state, state_feed] : view.each()) {
    check_gl_error("SystemRenderer start");

    if (reg.all_of<ToDestroy>(entity)) {
      DBG_LOG("SystemRenderer destroy\n");
      // Dispose passes resources
      deleteFrameBuffer(state.fb);
      destroyTexture(state.tex_id_y);
      destroyTexture(state.tex_id_u);
      destroyTexture(state.tex_id_v);
      state.tex_id_y = 0;
      state.tex_id_u = 0;
      state.tex_id_v = 0;
      // TODO: dispose component resources i.e. GLMeshComponent
      return;
    }

    if (!state.is_init) {
      state.is_init = true;
      DBG_LOG("SystemRenderer init: %zux%zu\n", state.screen_width,
              state.screen_height);

      auto viewMesh = reg.view<MeshComponent>();
      for (auto [entity, cmpMesh] : viewMesh.each()) {

        unsigned int VBO, VBO2, VAO, EBO;
        createVAO(cmpMesh.mesh, &VAO, &EBO, &VBO, &VBO2);

        auto& cmpMeshGL =
            reg.emplace<GLMeshComponent>(entity, VAO, VBO, VBO2, EBO);
      }

      size_t width = state_feed.width;
      size_t height = state_feed.height;
      createTextureR(state.tex_id_y, width, height);
      createTextureR(state.tex_id_u, width / 2, height / 2);
      createTextureR(state.tex_id_v, width / 2, height / 2);
      check_gl_error("SystemRenderer feed init");

      GLuint pFullscreen = loadProgram(kVS_FS, kFS_FS);
      GLuint pFullscreenYUV = loadProgram(kVS_FS, kFS_FS_YUV420);
      GLuint pCube = loadProgram(kVS_Cube, kFS_Cube);
      state.fb = createFrameBuffer(kFbWidth, kFbHeight);
      state.screenbuffer = FrameBuffer{
          .id = 0,
          .colorTarget = 0,
          .rboTarget = 0,
          .width = state.screen_width,
          .height = state.screen_height,
      };
   
      state.pass_fullscreen_yuv.init(state.fb, pFullscreenYUV,
                                   Uniforms{
                                       Res{
                                           .type = ResType::kTexture,
                                           .name = "s_textureY",
                                           .textureID = state.tex_id_y,
                                       },
                                       Res{
                                           .type = ResType::kTexture,
                                           .name = "s_textureU",
                                           .textureID = state.tex_id_u,
                                       },
                                       Res{
                                           .type = ResType::kTexture,
                                           .name = "s_textureV",
                                           .textureID = state.tex_id_v,
                                       },
                                       Res{
                                           .type = ResType::kFloat,
                                           .name = "scale_x",
                                           .floatValue = ((float)state.fb.width) / (float) state_feed.width,
                                       },
                                   });
      state.pass_geometry.init(state.fb, pCube);

      state.pass_fullscreen.init(state.screenbuffer, pFullscreen,
                                Uniforms{
                                    Res{
                                      .type = ResType::kTexture,
                                      .name = "s_tex",
                                      .textureID = state.fb.colorTarget
                                    },
                                    Res{
                                      .type = ResType::kFloat,
                                      .name = "scale_x",
                                      .floatValue = 1.,
                                    },
                                });
      check_gl_error("SystemRenderer init");
    }

    if (state_feed.is_dirty) {
      state_feed.is_dirty = false;
      auto width = (GLsizei)state_feed.width;
      auto height = (GLsizei)state_feed.height;
      updateTexture(state.tex_id_y, 0, width, height,
                    state_feed.p_data_y.get());
      updateTexture(state.tex_id_u, 1, width / 2, height / 2,
                    state_feed.p_data_u);
      updateTexture(state.tex_id_v, 2, width / 2, height / 2,
                    state_feed.p_data_v);
    }

    state.fb.bind();
    state.pass_clear.run();
    state.pass_fullscreen_yuv.run();
    state.pass_geometry.run(reg);

    state.screenbuffer.bind();
    state.pass_fullscreen.run();
    check_gl_error("SystemRenderer end");
  }

  auto finish = Clock::now();
  auto duration_ms =
      std::chrono::duration_cast<std::chrono::milliseconds>(finish - start)
          .count();
  auto duration_ns =
      std::chrono::duration_cast<std::chrono::nanoseconds>(finish - start)
          .count();
#if 0  // log frame time         
  DBG_LOG("SystemRenderer took %llu ms (took %llu ns)\n", duration_ms,
          duration_ns);
#endif
}
