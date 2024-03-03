#pragma once

#include <SDL.h>

struct SDL {
  SDL_Window* window = nullptr;
  int window_width = 0;
  int window_height = 0;
  SDL_GLContext glContext;
};

int sdl_init(SDL& sdl, int width, int height, bool is_headless = false);

void sdl_init_gl(SDL& sdl);

bool sdl_is_running();

void sdl_deinit(SDL& sdl);

void sdl_deinit_gl(SDL& sdl);
