#!/bin/bash
PROJECT_ROOT="$(dirname $(pwd))"

# build sub-topic program.
cd $PROJECT_ROOT/weibo_models
mkdir build; cd $_
cmake ..
make
cd $PROJECT_ROOT

# build Topic-mining and politics-topic mining
make -f myMakeFile
rm -rf ./src/*.o
