// GOAL
//  - bloat free portable AR engine
//    - mobile, linux backend
//    - fast iteration speed
//    - with tests
//
// Target structure:
//  legend: present, (planned)
//
//   (mlfx-core)-+-> mlfx -+-> (demo app) -+-> mlfx-standalone
//   (algo 1)   -+         |               +-> mlfx-standalone-android
//   (...)       |         |
//   (algo N)   -+         +->  mlfx-test -+-> mlfx-standalone-test
//                                         +-> mlfx-android-test
//
// TODO
//  - demo:
//    v algo style transfer: in image, style; out result
//    - algo silhouette segmentation: in image; out mask
//    - algo face3D: in face image; out head pose
//    - Face3D flow:
//      - cam +-> texture
//            +-> silhouette inference -> texture
//            +-> face landmark 3D inference -> mesh -> animate and morph
//
//  - end-to-end tests:
//    - NONE: exit 0
//    v GLES: render triangle
//    - HEADLESS GL: render triangle
//    - HEADLESS GLES: render triangle
//    - HEADLESS NONE: exit 0
//    v run on mobile?
//    - cleanup
//
//  - gfx:
//    v fullscreen
//    v unlit
//    - render to texture
//    - imgui dbg
//    - morph
//    - PBR
//

// - draw over video input
// v draw over camera input (mac)
// v tests

// - add compositing framegraph

// https://gist.github.com/phynet/90cfcf464c28c80e5247
// https://github.com/android/ndk-samples
// https://github.com/ochornenko/Camera2GLPreview
// https://registry.khronos.org/OpenGL/extensions/OES/OES_EGL_image_external.txt
// https://gist.github.com/rexguo/6696123
// https://stackoverflow.com/questions/30028156/using-video-stream-as-open-gl-es-2-0-texture

#pragma once

#include "utils.h"
#include "platform.h"
#include <chrono>
using Clock = std::chrono::high_resolution_clock;
using TimePoint = std::chrono::time_point<std::chrono::high_resolution_clock>;

#include "file_io.h"
#include "image.h"

#include "nodes/nodes.h"
