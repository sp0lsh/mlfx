#!/bin/bash

set -e

main() {
      ROOT_DIR=$(git rev-parse --show-toplevel)
      
      asset_filename=$@ 
      $ROOT_DIR/tools/bin2header --stdvector $ROOT_DIR/assets/$asset_filename
      
      header_filename=$(echo $asset_filename | sed -r 's/-/_/g')
      du -sh $ROOT_DIR/assets/$header_filename.h
      mv $ROOT_DIR/assets/$header_filename.h $ROOT_DIR/tests_standalone/src/asset_$header_filename.h
}

main $@
