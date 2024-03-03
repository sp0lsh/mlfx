
#include "image.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#include <opencv2/opencv.hpp>
#include <opencv2/imgproc.hpp> 

namespace cv {
  struct Mat;
} // namespace



int image_load(const std::string& path, Image& out_image) {
  // HERE_P(path)

  if (out_image.data != nullptr) {
    // HERE_P("Image not empty")
    return 1;
  }

  // Load the image
  stbi_set_flip_vertically_on_load(true);
  int picWidth, picHeight, n;
  out_image.data = stbi_load(path.c_str(), &picWidth, &picHeight, &n, 4);
  if (out_image.data == nullptr) {
    // HERE_P("Failed to load image" << stbi_failure_reason())
    return 1;
  }

  out_image.width = picWidth;
  out_image.height = picHeight;

  return 0;
}

int image_load_mem(const char* src, size_t size, Image& out_image) {
  // HERE_P(path)

  if (out_image.data != nullptr) {
    // HERE_P("Image not empty")
    return 1;
  }

  // Load the image
  stbi_set_flip_vertically_on_load(true);
  int w, h, n;
  out_image.data =
      stbi_load_from_memory((unsigned char*)src, size, &w, &h, &n, 4);
  if (out_image.data == nullptr) {
    // HERE_P("Failed to load image" << stbi_failure_reason())
    return 1;
  }

  out_image.width = w;
  out_image.height = h;

  return 0;
}

int image_unload_stb(Image& image) {
  if (image.data == nullptr) {
    // HERE_P("Image empty")
    return 1;
  }

  stbi_image_free(image.data);  // Free the reference to the data
  image.data = nullptr;
  image.width = 0;
  image.height = 0;

  return 0;
}

void create_ppm(const char* prefix,
                unsigned int width,
                unsigned int height,
                unsigned int pixel_nbytes,
                uint8_t* pixels) {
  size_t i, j, k, cur;
  constexpr size_t max_filename = 256;
  char filename[max_filename];
  snprintf(filename, max_filename, "%s.ppm", prefix);
  FILE* f = fopen(filename, "w");
  fprintf(f, "P3\n%d %d\n%d\n", width, height, 255);
  for (i = 0; i < height; i++) {
    for (j = 0; j < width; j++) {
      cur = pixel_nbytes * ((height - i - 1) * width + j);
      fprintf(f, "%3d %3d %3d ", pixels[cur], pixels[cur + 1], pixels[cur + 2]);
    }
    fprintf(f, "\n");
  }
  fclose(f);
}

void create_ppm_fp32(const char* prefix,
                     unsigned int width,
                     unsigned int height,
                     unsigned int pixel_nbytes,
                     float* pixels) {
  size_t i, j, k, cur;
  constexpr size_t max_filename = 256;
  char filename[max_filename];
  snprintf(filename, max_filename, "%s.ppm", prefix);
  FILE* f = fopen(filename, "w");
  fprintf(f, "P3\n%d %d\n%d\n", width, height, 255);
  for (i = 0; i < height; i++) {
    for (j = 0; j < width; j++) {
      cur = pixel_nbytes * ((height - i - 1) * width + j);
      fprintf(f, "%3d %3d %3d ", (uint8_t)(pixels[cur + 0] * 255.f),
              (uint8_t)(pixels[cur + 1] * 255.f),
              (uint8_t)(pixels[cur + 2] * 255.f));
    }
    fprintf(f, "\n");
  }
  fclose(f);
}

void image_float32_from_uint8(float* dst,
                              const uint8_t* src,
                              const int w,
                              const int h) {
  float* buf_fp32 = dst;
  const uint8_t* buf_ui8 = src;
  float mean = 0.0f;
  float std = 255.0f;
  int idx_src = 0;
  int idx_dst = 0;
  int size = w * h;
  for (int i = 0; i < size; ++i) {
    int r = buf_ui8[idx_src + 0];
    int g = buf_ui8[idx_src + 1];
    int b = buf_ui8[idx_src + 2];
    // buf_fp32[idx_dst + 0] = 1.0f;
    buf_fp32[idx_dst + 0] = (float)(r - mean) / std;
    buf_fp32[idx_dst + 1] = (float)(g - mean) / std;
    buf_fp32[idx_dst + 2] = (float)(b - mean) / std;
    idx_src += 4;
    idx_dst += 3;
  }
}

void image_fp32_to_uint8(float* src, uint8_t* dst, int img_w, int img_h) {
  uint8_t* d = dst;
  float* s = src;

  for (int y = 0; y < img_h; y++) {
    for (int x = 0; x < img_w; x++) {
      float r = *s++;
      float g = *s++;
      float b = *s++;
      *d++ = (uint8_t)(r * 255);
      *d++ = (uint8_t)(g * 255);
      *d++ = (uint8_t)(b * 255);
      *d++ = 0xFF;
    }
  }
}

void mat_bgr_to_packed_yuv(uint8_t* data, const cv::Mat& cam_bgr) {
    static cv::Mat cam_yuv;
    cv::cvtColor(cam_bgr, cam_yuv, cv::COLOR_BGR2YUV);
    mat_to_packed(data, cam_yuv);
}

void mat_to_packed(uint8_t* data, const cv::Mat& cam_bgr) {
    static std::vector<cv::Mat> channels_bgr(3);
    cv::split(cam_bgr, channels_bgr);

    static cv::Mat gHalf(cam_bgr.cols / 2, cam_bgr.rows / 2, CV_8UC1);
    static cv::Mat rHalf(cam_bgr.cols / 2, cam_bgr.rows / 2, CV_8UC1);
    cv::resize(channels_bgr[1], gHalf, cv::Size(0,0), 0.5, 0.5, cv::INTER_LINEAR);
    cv::resize(channels_bgr[2], rHalf, cv::Size(0,0), 0.5, 0.5, cv::INTER_LINEAR);

    size_t size_b = cam_bgr.rows * cam_bgr.cols;
    size_t size_g = cam_bgr.rows * cam_bgr.cols / 4;
    size_t size_r = cam_bgr.rows * cam_bgr.cols / 4;

    memcpy(data, channels_bgr[0].data, size_b);
    memcpy(data + size_b, gHalf.data, size_g);
    memcpy(data + size_b + size_g, rHalf.data, size_r);
}