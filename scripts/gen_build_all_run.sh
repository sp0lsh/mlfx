#!/bin/bash

set -e

main() {

      ROOT_DIR=$(git rev-parse --show-toplevel)
      cd $ROOT_DIR/scripts

      # convinience, regenerate project files
      if (( $# > 0 )); then
            rm -rf ./build*
            ./download_assets.sh
      fi

      ./gen_build_lib_android.sh
      ./gen_build_lib_android_test.sh
      
      ./gen_build_standalone_test.sh
      ./gen_build_standalone.sh -r -t # r: run, t: kill after 5 sec

      cd $ROOT_DIR
      ./android/build.sh
      ./android/build_and_run.sh
      sleep 10
      ./android/stop.sh
}

main $@
