#/bin/bash

set -e

main() {
      ROOT_DIR=$(git rev-parse --show-toplevel)
      cd $ROOT_DIR
      
      export PATH="$PATH:$ANDROID_HOME/platform-tools"

      TEST_TARGET=mlfx-android-test
      BUILD_DIR=./build_android

      # Create a separate folder for test files
      TEST_DIR=mlfx-test-package
      ANDROID_TEMP=/data/local/tmp
      ANDROID_TEMP_DIR="$ANDROID_TEMP/$TEST_DIR"
      adb shell mkdir -p $ANDROID_TEMP_DIR

      # Push files
      echo "Pushing: ./$BUILD_DIR/android_test/$TEST_TARGET -> $ANDROID_TEMP_DIR"
      adb push --sync ./$BUILD_DIR/android_test/$TEST_TARGET $ANDROID_TEMP_DIR
      # adb push --sync ./android/nativeLibs/**/armeabi-v7a/lib*.so "$ANDROID_TEMP_DIR"
      # adb push --sync ./assets/scene/* "$ANDROID_TEMP_DIR"
      # adb push --sync ./assets/shaders/* "$ANDROID_TEMP_DIR"
      # adb push --sync ./armeabi-v7a/libassimp.so "$ANDROID_TEMP_DIR"

      # Run tests
      set +e
      echo "======== TEST RUN ============================================================="
      # adb shell "LD_LIBRARY_PATH=$ANDROID_TEMP_DIR BASE_DIR=$ANDROID_TEMP_DIR $ANDROID_TEMP_DIR/$TEST_TARGET --out $ANDROID_TEMP_DIR/result.xml"
      adb shell "LD_LIBRARY_PATH=$ANDROID_TEMP_DIR BASE_DIR=$ANDROID_TEMP_DIR $ANDROID_TEMP_DIR/$TEST_TARGET --out $ANDROID_TEMP_DIR/result.xml"
      test_status=$?
      echo "==============================================================================="

      # Pull reports
      echo "Pull: ./$BUILD_DIR/$TEST_DIR <- $ANDROID_TEMP_DIR"
      mkdir -p $BUILD_DIR
      adb shell ls -R "$ANDROID_TEMP_DIR"
      adb pull $ANDROID_TEMP_DIR ./$BUILD_DIR

      # Clean folder
      adb shell rm -rf $ANDROID_TEMP_DIR

      # Report
      echo
      echo "======== TEST REPORT =========================================================="
      echo "cat $BUILD_DIR/$TEST_DIR/result.xml"
      cat $BUILD_DIR/$TEST_DIR/result.xml
      echo "==============================================================================="

      exit $test_status
}

main
