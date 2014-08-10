#!/bin/bash
rm libprocess.a
ar x libICTCLAS50.a
ar x libboost_regex.a
ar rcs libprocess.a *.o
rm *.o

