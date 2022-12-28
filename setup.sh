#!/bin/bash
# Cleanup any old Cmake artifacts
if [ -d "build" ];then rm -rf build; fi

echo "Generating Makefile..."
mkdir -p build
cd build
cmake -G "Unix Makefiles" -DCMAKE_TOOLCHAIN_FILE="../cmake/avr-gcc-toolchain.cmake" ..

exit 0