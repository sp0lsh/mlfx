#pragma once

#include <ctime>
#include <string>

namespace cv {
  class Mat;
} // namespace


struct Image {
  unsigned char* data{nullptr};
  int width{0};
  int height{0};
};

int image_load(const std::string& path, Image& out_image);

int image_load_mem(const char* src, size_t size, Image& out_image);

int image_unload_stb(Image& image);

void create_ppm(const char* prefix,
                unsigned int width,
                unsigned int height,
                unsigned int pixel_nbytes,
                uint8_t* pixels);

void create_ppm_fp32(const char* prefix,
                     unsigned int width,
                     unsigned int height,
                     unsigned int pixel_nbytes,
                     float* pixels);

void image_float32_from_uint8(float* dst,
                              const uint8_t* src,
                              const int w,
                              const int h);

void image_fp32_to_uint8(float* src, uint8_t* dst, int img_w, int img_h);

void mat_bgr_to_packed_yuv(uint8_t* data, const cv::Mat& cam_bgr);

void mat_to_packed(uint8_t* data, const cv::Mat& cam_bgr);
