#!/bin/bash
PROJECT_ROOT="$(dirname $(pwd))"

# build data cleaner program
cd $PROJECT_ROOT/weibo_ad_filter
mkdir build; cd $_
cmake ..
make

# build sub-topic program.
cd $PROJECT_ROOT/weibo_models
mkdir build; cd $_
cmake ..
make

# build Topic-mining and politics-topic mining
cd $PROJECT_ROOT/DataProcess
mkdir build; cd $_
cmake ..
make
cd $PROJECT_ROOT
