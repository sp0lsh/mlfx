#!/bin/bash

set -e

main() {

      ROOT_DIR=$(git rev-parse --show-toplevel)
      cd $ROOT_DIR

      BUILD_TYPE=Release # Release, Debug
      SYSTEM_PROCESSOR=armv7 # arm, armv7

      CMAKE_TOOLCHAIN_FILE=$ANDROID_NDK_HOME/build/cmake/android.toolchain.cmake
      ANDROID_ABI=armeabi-v7a # armeabi-v7a(+NEON), arm64-v8a, simulator: x86, x86_64
      MINSDKVERSION=26
      ANDROID_ARM_NEON=TRUE # TRUE, FALSE
      ANDROID_STL=c++_static # c++_static, c++_shared, none, system
      ANDROID_TOOLCHAIN=clang

      # convinience, regenerate project files
      # rm -rf ./build/*
      
      # build and run
      mkdir -p ./build_android
      cd ./build_android

      # angle
      # set DYLD_LIBRARY_PATH="/Users/mk/dev/angle_sdl_test/lib/macos/"
      # bazel build //tensorflow/lite:libtensorflowlite.so

      # https://developer.android.com/ndk/guides/cmake#command-line
      cmake \
            -S .. \
            -B .  \
            -G Ninja \
            -DMLFX_OS="android" \
            -DMLFX_IS_TEST=False \
            -DCMAKE_BUILD_TYPE=$BUILD_TYPE \
            -DCMAKE_INSTALL_RPATH_USE_LINK_PATH="ON" \
            -DTENSORFLOW_SOURCE_DIR="/Users/mk/dev2/tensorflow" \
            -DCMAKE_SYSTEM_PROCESSOR=$SYSTEM_PROCESSOR \
            -DCMAKE_TOOLCHAIN_FILE=$CMAKE_TOOLCHAIN_FILE \
            -DANDROID_TOOLCHAIN=$ANDROID_TOOLCHAIN \
            -DANDROID_ABI=$ANDROID_ABI \
            -DANDROID_ARM_NEON=$ANDROID_ARM_NEON \
            -DANDROID_PLATFORM=android-$MINSDKVERSION \
            -DANDROID_STL=$ANDROID_STL \
            -Dxnn_enable_arm_i8mm=false

      ninja -j16

      # ./size_raport.sh
      du -sh ./mlfx/libmlfx-lib.a

      # publish
      echo "Publishing library..."
      # lib
      mkdir -p $ROOT_DIR/release/lib/$ANDROID_ABI
      cp mlfx/libmlfx-lib.a $ROOT_DIR/release/lib/$ANDROID_ABI/libmlfx-lib.a
      # include
      mkdir -p $ROOT_DIR/release/
      cp -R $ROOT_DIR/mlfx/include $ROOT_DIR/release
      ls -R $ROOT_DIR/release
}

main $@
