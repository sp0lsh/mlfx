
#include "sdl.h"

#include <angle_gl.h>
#include <mlfx/engine.h>
#include <mlfx/passes.h>
#include <mlfx/image.h>

#include <opencv2/opencv.hpp> // cv::Mat, cv::VideoCapture

#include <iostream>

namespace {

constexpr size_t kWindowWidth = 512;
constexpr size_t kWindowHeight = 1024;
// constexpr size_t kWindowWidth = 1920/2;
// constexpr size_t kWindowHeight = 1080/2;

}  // namespace

int main(int, char**) {
  SDL sdl;
  if (int ok = sdl_init(sdl, kWindowWidth, kWindowHeight); ok != 0)
    return EXIT_FAILURE;
  sdl_init_gl(sdl);

  cv::VideoCapture cam(0);
  cv::Mat cam_bgr;
  cam >> cam_bgr;

  std::unique_ptr<Engine> pEngine = std::make_unique<Engine>();
  pEngine->init(nullptr, sdl.window_width, sdl.window_height);

  // for (size_t i = 0; i < 10; ++i) {
  while (sdl_is_running()) {
    {
      if (!cam.isOpened()) {
          std::cout << "Failed to make connection to cam" << std::endl;
          cam.open(0);
      }
      
      cam >> cam_bgr;
      // clang-format off
      size_t data_len =  cam_bgr.rows * cam_bgr.cols 
                      + (cam_bgr.rows * cam_bgr.cols / 4)
                      + (cam_bgr.rows * cam_bgr.cols / 4);
      // clang-format on
      uint8_t data[data_len];
      mat_bgr_to_packed_yuv(data, cam_bgr);
      pEngine->draw(data, data_len, cam_bgr.cols, cam_bgr.rows, 0.0f);
    }
    
    pEngine->render();
    SDL_GL_SwapWindow(sdl.window);
  }

  sdl_deinit_gl(sdl);
  sdl_deinit(sdl);
  return EXIT_SUCCESS;
}
