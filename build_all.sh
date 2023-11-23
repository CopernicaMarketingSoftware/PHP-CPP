#!/bin/bash

# 
# Run this script from project root dir
# 

echo --------------------------------------------------------
echo -- Building Shared Library - Debug 
echo --------------------------------------------------------
cmake -S . -B build_shared_d -G "Unix Makefiles" \
-DPHPCPP_BUILD_SHARED=TRUE \
-DCMAKE_BUILD_TYPE="Debug"

sleep 5
cmake --build build_shared_d
sleep 5



echo --------------------------------------------------------
echo -- Building Shared Library - Release 
echo --------------------------------------------------------
cmake -S . -B build_shared_r -G "Unix Makefiles" \
-DPHPCPP_BUILD_SHARED=TRUE \
-DCMAKE_BUILD_TYPE="Release"

sleep 5
cmake --build build_shared_r
sleep 5



echo --------------------------------------------------------
echo -- Building Static Library - Debug 
echo --------------------------------------------------------
cmake -S . -B build_static_d -G "Unix Makefiles" \
-DPHPCPP_BUILD_SHARED=FALSE \
-DCMAKE_BUILD_TYPE="Debug"

sleep 5
cmake --build build_static_d
sleep 5



echo --------------------------------------------------------
echo -- Building Static Library - Release 
echo --------------------------------------------------------
cmake -S . -B build_static_r -G "Unix Makefiles" \
-DPHPCPP_BUILD_SHARED=FALSE \
-DCMAKE_BUILD_TYPE="Release"

sleep 5
cmake --build build_static_r
sleep 5
