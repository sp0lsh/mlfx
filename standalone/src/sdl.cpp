#include "sdl.h"

#include <angle_gl.h>
#include <iostream>

namespace {

constexpr char kTitle[] = "MLFX - Standalone (SDL, OpenGL ES 3.0 via Angle)";

}  // namespace

int sdl_init(SDL& sdl, int width, int height, bool is_headless /* = false */) {
  // Init SDL
  if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_EVENTS) < 0) {
    std::cerr << SDL_GetError() << std::endl;
    return 1;
  }

  SDL_version version;
  SDL_GetVersion(&version);
  std::cout << "SDL version: " << static_cast<int>(version.major) << "."
            << static_cast<int>(version.minor) << "."
            << static_cast<int>(version.patch) << std::endl;

  // Create window
  SDL_SetHint(SDL_HINT_OPENGL_ES_DRIVER, "1");
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_EGL, 1);
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_ES);

  // Explicitly set channel depths, otherwise we might get some < 8
  SDL_GL_SetAttribute(SDL_GL_RED_SIZE, 8);
  SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, 8);
  SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, 8);
  SDL_GL_SetAttribute(SDL_GL_ALPHA_SIZE, 8);
  SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 16);
  SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);

  // macos retina:
  // https://stackoverflow.com/questions/18544881/sdl-2-0-retina-mac
  auto window_flag = is_headless ? SDL_WINDOW_HIDDEN : SDL_WINDOW_SHOWN;
  auto windowFlags = window_flag | SDL_WINDOW_OPENGL | SDL_WINDOW_ALLOW_HIGHDPI;
  sdl.window =
      SDL_CreateWindow(kTitle, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
                       width, height, windowFlags);
  SDL_GL_GetDrawableSize(sdl.window, &sdl.window_width, &sdl.window_height);

  return 0;
}

void sdl_init_gl(SDL& sdl) {
  SDL_GL_SetSwapInterval(1);
  sdl.glContext = SDL_GL_CreateContext(sdl.window);
  SDL_GL_MakeCurrent(sdl.window, sdl.glContext);
  std::cout << "GL version: " << glGetString(GL_VERSION) << std::endl;
  std::cout << "GL renderer: " << glGetString(GL_RENDERER) << std::endl;
  std::cout << "GL vendor: " << glGetString(GL_VENDOR) << std::endl;
  std::cout << "GL glsl version: " << glGetString(GL_SHADING_LANGUAGE_VERSION)
            << std::endl;
  SDL_GL_SetSwapInterval(1);
}

bool sdl_is_running() {
  SDL_Event event;
  while (0 != SDL_PollEvent(&event)) {
    if (SDL_QUIT == event.type) {
      return false;
    } else if (SDL_KEYDOWN == event.type) {
      const auto keyStates_p = SDL_GetKeyboardState(nullptr);
      if (keyStates_p[SDL_SCANCODE_ESCAPE]) {
        return false;
      }
    }
  }
  return true;
}

void sdl_deinit(SDL& sdl) {
  SDL_DestroyWindow(sdl.window);
  SDL_Quit();
}

void sdl_deinit_gl(SDL& sdl) {
  SDL_GL_DeleteContext(sdl.glContext);
}
