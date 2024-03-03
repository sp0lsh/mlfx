#!/bin/bash

set -e

main() {
     mkdir -p ./third_parties
     pushd ./third_parties
     
     # if file not exists
     if [ ! -f ./opencv-android-sdk.zip ]; then
          echo "SDK file not found! Downloading..."
          wget --no-clobber -O opencv-android-sdk.zip https://github.com/opencv/opencv/releases/download/4.8.1/opencv-4.8.1-android-sdk.zip
     fi

     # if dir not exists
     if [ ! -d ./opencv-android-sdk ]; then
          echo "SDK dir not found! Extracting..."
          tar -xf ./opencv-android-sdk.zip
          mv OpenCV-android-sdk opencv-android-sdk
     fi
     
     # sdk is 700MB by default, remove the bloat
     if [ -d ./opencv-android-sdk ]; then
          echo "Removing the unused bloat..."
          pushd ./opencv-android-sdk
          rm -rf samples
          rm -rf sdk/java
          popd
     fi

     popd
}

main
