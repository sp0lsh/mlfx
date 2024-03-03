#!/bin/bash

set -e

main() {
     ROOT_DIR=$(git rev-parse --show-toplevel)
     cd $ROOT_DIR

     brew install ninja cmake opencv sdl
     $ROOT_DIR/scripts/third_parties_angle.sh
}

main
