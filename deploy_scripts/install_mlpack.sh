#!/bin/bash
sudo yum install libxml2-devel armadillo-devel
wget http://www.mlpack.org/files/mlpack-1.0.9.tar.gz
tar -xf mlpack-1.0.9.tar.gz
cd mlpack-1.0.9
mkdir build; cd $_
cmake ..
make
sudo make install
cd ../..
sudo rm -rf mlpack*
