#!/bin/bash

set -e

main() {

      # for macos
      brew install wget

      wget --no-clobber -P ./assets https://storage.googleapis.com/khanhlvg-public.appspot.com/arbitrary-style-transfer/belfry-2611573_1280.jpg
      wget --no-clobber -P ./assets https://storage.googleapis.com/khanhlvg-public.appspot.com/arbitrary-style-transfer/style23.jpg

      wget --no-clobber \
            -O ./assets/magenta_arbitrary-image-stylization-v1-256_int8_prediction_1.tflite\
            https://tfhub.dev/google/lite-model/magenta/arbitrary-image-stylization-v1-256/int8/prediction/1?lite-format=tflite

      wget --no-clobber \
            -O ./assets/magenta_arbitrary-image-stylization-v1-256_int8_transfer_1.tflite \
            https://tfhub.dev/google/lite-model/magenta/arbitrary-image-stylization-v1-256/int8/transfer/1?lite-format=tflite

      wget --no-clobber \
            -O ./assets/magenta_arbitrary-image-stylization-v1-256_fp16_prediction_1.tflite\
            https://tfhub.dev/google/lite-model/magenta/arbitrary-image-stylization-v1-256/fp16/prediction/1?lite-format=tflite

      wget --no-clobber \
            -O ./assets/magenta_arbitrary-image-stylization-v1-256_fp16_transfer_1.tflite \
            https://tfhub.dev/google/lite-model/magenta/arbitrary-image-stylization-v1-256/fp16/transfer/1?lite-format=tflite
}

main
