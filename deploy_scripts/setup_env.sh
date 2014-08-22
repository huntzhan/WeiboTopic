#!/bin/env bash
alias g++48=/opt/rh/devtoolset-2/root/usr/bin/g++
alias gcc48=/opt/rh/devtoolset-2/root/usr/bin/gcc
export CXX=/opt/rh/devtoolset-2/root/usr/bin/g++
export CC=/opt/rh/devtoolset-2/root/usr/bin/gcc
export LD_LIBRARY_PATH=/usr/lib64:/usr/lib:/usr/local/lib:$LD_LIBRARY_PATH

alias g++=g++48
alias gcc=gcc48
alias cmake=cmake28
