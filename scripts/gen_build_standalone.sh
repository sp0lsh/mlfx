#!/bin/bash

set -e

print_usage() {
      echo "Usage:"
      echo "    -c: clean"
      echo "    -r: run after build"
      echo "    -t: timeout, exit run after $RUN_TIMEOUT_SEC sec"
      echo "    any other: print this help"
}

get_flags() {
      # https://stackoverflow.com/questions/7069682/how-to-get-arguments-with-flags-in-bash

      while getopts 'crt' flag; do
            case "${flag}" in
                  c) flag_clean=true ;;
                  r) flag_run=true ;;
                  t) flag_timeout=true ;;
                  *) print_usage
                        exit 1 ;;
            esac
      done
}

clean() {
      if [ "$flag_clean" = true ]; then
            # force regenerate project files
            rm -rf ./build/*
      fi
}

generate() {
      mkdir -p ./build
      cd ./build

      # bazel build //tensorflow/lite:libtensorflowlite.so

      cmake -S .. \
            -B .  \
            -G Ninja \
            -DMLFX_OS="darwin" \
            -DMLFX_IS_TEST=False \
            -DCMAKE_INSTALL_RPATH_USE_LINK_PATH="ON" \
            -DTENSORFLOW_SOURCE_DIR="/Users/mk/dev2/tensorflow"
}

run() {
      if [ "$flag_run" = true ]; then
            echo ""
            echo ""
            export DYLD_LIBRARY_PATH="$ROOT_DIR/third_parties/angle-lib/lib/macos/"

            if [ "$flag_timeout" = true ]; then
                  # run, kill after RUN_TIMEOUT_SEC (5 by default) sec
                  ./standalone/mlfx-standalone & sleep $RUN_TIMEOUT_SEC; kill $!
            else
                  ./standalone/mlfx-standalone
            fi
      fi
}

main() {
      ROOT_DIR=$(git rev-parse --show-toplevel)
      cd $ROOT_DIR

      RUN_TIMEOUT_SEC=5 # sec
      flag_clean=false
      flag_run=false
      flag_timeout=false
      get_flags $@

      clean
      generate
      ninja -j16 # build
      run
}

main $@
