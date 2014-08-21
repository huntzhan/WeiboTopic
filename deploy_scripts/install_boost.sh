#!/bin/env bash

# install boost.
wget -O boost_1_56_0.zip http://sourceforge.net/projects/boost/files/boost/1.56.0/boost_1_56_0.zip/download
unzip boost_1_56_0.zip
cd boost_1_56_0
./bootstrap.sh
sudo ./b2 install --prefix=/usr
cd ..
sudo rm -rf boost_1_56_0*
