#!/bin/bash

set -e

main() {

      ROOT_DIR=$(git rev-parse --show-toplevel)
      cd $ROOT_DIR
      
      # fetch all submodules
      # git submodule update --init --recursive

      if (( $# > 0 )); then # --clean
            # convinience, regenerate project files
            rm -rf ./build/*

            # TODO: Loop over the assets?
            ./gen_inline_asset.sh magenta_arbitrary-image-stylization-v1-256_fp16_prediction_1.tflite
            ./gen_inline_asset.sh magenta_arbitrary-image-stylization-v1-256_fp16_transfer_1.tflite
            ./gen_inline_asset.sh style23.jpg
            ./gen_inline_asset.sh belfry-2611573_1280.jpg
      fi

      # build and run
      mkdir -p ./build
      cd ./build

      # angle
      # set DYLD_LIBRARY_PATH="/Users/mk/dev/angle_sdl_test/lib/macos/"
      # bazel build //tensorflow/lite:libtensorflowlite.so

      cmake -S .. \
            -B .  \
            -G Ninja \
            -DMLFX_OS="darwin" \
            -DMLFX_IS_TEST=True \
            -DCMAKE_INSTALL_RPATH_USE_LINK_PATH="ON" \
            -DTENSORFLOW_SOURCE_DIR="/Users/mk/dev2/tensorflow"


      ninja -j16

      export DYLD_LIBRARY_PATH="$ROOT_DIR/third_parties/angle-lib/lib/macos/"
      ./standalone_test/mlfx-standalone-test

      # ./size_raport.sh
      du -sh ./standalone_test/mlfx-standalone-test
}

main $@
