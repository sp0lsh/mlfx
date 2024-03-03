#!/bin/bash

set -e

main() {

      ROOT_DIR=$(git rev-parse --show-toplevel)
      cd $ROOT_DIR/scripts

      if (( $# > 0 )); then # --clean
            # convinience, regenerate project files
            rm -rf ./build*
            ./download_assets.sh
      fi

      ./gen_build_lib_android.sh
      ./gen_build_lib_android_test.sh
      
      ./gen_build_standalone_test.sh
      ./gen_build_standalone.sh

      cd $ROOT_DIR
      ./android/build.sh
}

main $@
