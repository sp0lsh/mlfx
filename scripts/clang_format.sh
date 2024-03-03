#!/bin/bash

set -e

format_file() {
      file=$1

      # blacklist heavy files for faster formatting
      blacklist=(
            .*catch2.hpp
            .*entt.hpp
            .*.jpg.h
            .*.bin.h
            .*.tflite.h
      )

      for item in "${blacklist[@]}"; do
            if [[ "$file" =~ $item ]]; then
                  return # skip file
            fi
      done

      # Build-in styles comparison:
      # https://clang.llvm.org/docs/ClangFormatStyleOptions.html
      # https://gist.github.com/andrewseidl/8066c18e97c40086c183

      clang-format -style=file -i $file
}

find_files() {     
      # run clang-format on all sources
      # https://leimao.github.io/blog/Clang-Format-Quick-Tutorial/
      # https://stackoverflow.com/questions/28896909/how-to-call-clang-format-over-a-cpp-project-folder#comment117427076_48459965
      
      # work with find -exec
      # https://www.baeldung.com/linux/find-exec-command
      export -f format_file
      find -E . -regex '.*\.(cpp|h|hpp|cc|cxx)' -exec bash -c "format_file \"{}\"" \;
}

main() {
      ROOT_DIR=$(git rev-parse --show-toplevel)
      cd $ROOT_DIR

      clang-format --version

      dirs=(
            mlfx/src
            mlfx_test/src
            standalone/src
            standalone_test/src
            android/app/src/main/cpp
      )

      for dir in "${dirs[@]}"; do
            pushd $dir
            find_files
            popd
      done
}

main
