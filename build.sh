#!/bin/bash

#define
ROOT_PATH=$PWD
BUILD_PATH=${ROOT_PATH}/build

#prepare dir
rm -rf ${BUILD_PATH}
mkdir -p ${BUILD_PATH}

cd ${BUILD_PATH}
cmake -DCMAKE_BUILD_TYPE=Debug ..
make -j5
