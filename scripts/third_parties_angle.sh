#!/bin/bash

set -e

# https://medium.com/@grplyler/building-and-linking-googles-angle-with-raylib-on-macos-67b07cd380a3
main() {
      ROOT_DIR=$(git rev-parse --show-toplevel)
      cd $ROOT_DIR

      # install gn: https://gn.googlesource.com/gn/
      if ! command -v gn &> /dev/null
      then
            echo "gn could not be found, installing..."

            pushd $ROOT_DIR/tools
            if [ ! -f gn.zip ]; then
                  wget -O gn.zip https://chrome-infra-packages.appspot.com/dl/gn/gn/mac-amd64/+/latest
                  tar -xf gn.zip
                  rm -rf .cipdpkg
            fi
      
            chmod +x gn
            PATH=$PATH:$ROOT_DIR/tools/

            popd
      fi

      mkdir -p third_parties
      cd third_parties

      mkdir -p angle
      cd angle
      # TODO: detect if empty (need fetch) or already downloaded (use glient sync) 
      # gclient sync
      fetch angle

      mkdir -p out/Relaase
      cp $ROOT_DIR/third_parties/angle_args.gn out/Relaase/args.gn      
      gn gen out/Relaase

      ninja -j16 -k1 -C out/Relaase

      # TODO: copy includes
      # copy libEGL.dylib and libGLESv2.dylib libs to angle-lib
      cp $ROOT_DIR/third_parties/angle/out/Relaase/libEGL.dylib \
         $ROOT_DIR/third_parties/angle-lib/lib/macos/libGLESv2.dylib
      cp $ROOT_DIR/third_parties/angle/out/Relaase//libGLESv2.dylib \
         $ROOT_DIR/third_parties/angle-lib/lib/macos//libGLESv2.dylib
}

main
