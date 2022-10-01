PHP-CPP-cmake
=============
This is a fork of PHP-CPP, with goal to enable
building PHP-CPP with Cmake on Windows and Linux.

Change lists: [Modifications.md](https://github.com/jefrisibarani/PHP-CPP-cmake/blob/master/Modifications.md)

Sample PHP-CPP extension project: [PHP-CPP-SampleExtension](https://github.com/jefrisibarani/PHP-CPP-SampleExtension)

Sample PHP-CPP extension project, using PHP-CPP-cmake as sub directory: [PHP-CPP-SampleExtension2](https://github.com/jefrisibarani/PHP-CPP-SampleExtension2)

## Building with command line
Download project source, then from project source root dir, run cmake 

## Build on windows
Dowload php-dev from https://windows.php.net/download/, then extract to a folder for example: d:/dev/cpp_php/ts/php-7.4.30-devel-vc15-x64
```
cmake -S . -B build_shared ^
  -G "Visual Studio 15 2017" -T host=x64 -A x64 ^
  -DPHPCPP_BUILD_SHARED=TRUE ^
  -DPHP_DEV_DIR="d:/dev/cpp_php/ts/php-7.4.30-devel-vc15-x64" ^
  -DPHPCPP_COMPILE_TS=TRUE ^
  -DPHPCPP_ARCH="x86_64"
```  
```
cmake --build build_shared --config=Release
```
```
cmake --build build_shared --config=Debug
```
## Build on linux
We need php-dev package installed.

Shared library - debug 
```
cmake -S . -B build_shared_d -G "Unix Makefiles" \
   -DPHPCPP_BUILD_SHARED=TRUE \
   -DCMAKE_BUILD_TYPE="Debug"
```
```
cmake --build build_shared_d
```

Shared library - release 
```
cmake -S . -B build_shared_r -G "Unix Makefiles" \
   -DPHPCPP_BUILD_SHARED=TRUE \
   -DCMAKE_BUILD_TYPE="Release"
```
```
cmake --build build_shared_r
```



## Successfully built on the following hosts:

***Windows 11***
- Cmake version 3.22.2
- Visual Studio 2017 (v141)
- MSVC 14.16.27023
- Windows SDK Version 10.0.19041.0
- PHP 7.4.30 (TS) with development package from https://windows.php.net/download/

***CentOS 7.9***
- CMake 3.17.5
- GCC 7.3.1 from  devtoolset-7
- PHP 7.2.34 (NTS)