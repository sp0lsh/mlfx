#!/bin/bash

PACKAGE=com.media.camera.preview

adb shell am kill $PACKAGE
adb shell am force-stop $PACKAGE
