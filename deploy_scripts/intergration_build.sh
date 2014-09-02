#!/bin/bash
PROJECT_ROOT="$(dirname $(pwd))"

# build sub-topic program.
cd $PROJECT_ROOT/weibo_models
mkdir build; cd $_
cmake ..
make
cd $PROJECT_ROOT
