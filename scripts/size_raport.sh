#!/bin/bash

set -e

main() {
      ROOT_DIR=$(git rev-parse --show-toplevel)
      cd $ROOT_DIR

      pushd ./build/tests_standalone/

      du -sh mlfx-tests

      dsymutil mlfx-tests
      strip mlfx-tests

      $ROOT_DIR/tools/bloaty -d compileunits \
            --debug-file=./mlfx-tests.dSYM/Contents/Resources/DWARF/mlfx-tests \
            mlfx-tests \
            > mlfx-tests_compileunits.log

      $ROOT_DIR/tools/bloaty -d symbols \
            --debug-file=./mlfx-tests.dSYM/Contents/Resources/DWARF/mlfx-tests \
            mlfx-tests \
            > mlfx-tests_symbols.log
      popd
}

main
