@echo off

REM
REM Build extension with PHP-CPP static/shared, Release/Debug build 
REM
REM Adapt the paths PHP_DEV_DIR, PHPCPP_COMPILE_TS and PHPCPP_ARCH
REM Run this script from project root dir
REM
REM To build with Visual Studio 2022, replace "Visual Studio 15 2017"
REM On line 24 and 54 with "Visual Studio 17 2022" 
REM
REM Author: jefrisibarani@gmail.com
REM


SET PHP_DEV_DIR="d:/dev/cpp_php/ts/php-7.4.30-devel-vc15-x64"
SET PHPCPP_COMPILE_TS=TRUE
SET PHPCPP_ARCH="x86_64"


echo --------------------------------------------------------
echo -- Configuring PHP-CPP shared library
echo --------------------------------------------------------
cmake -S . -B build_shared ^
-G "Visual Studio 15 2017" -T host=x64 -A x64 ^
-DPHPCPP_BUILD_SHARED=TRUE ^
-DPHP_DEV_DIR=%PHP_DEV_DIR% ^
-DPHPCPP_COMPILE_TS=%PHPCPP_COMPILE_TS% ^
-DPHPCPP_ARCH=%PHPCPP_ARCH%


timeout 3 > NUL
echo --------------------------------------------------------
echo -- Building Shared Library - Release 
echo --------------------------------------------------------
cmake --build build_shared --config=Release


timeout 3 > NUL
echo --------------------------------------------------------
echo -- Building Shared Library - Debug 
echo --------------------------------------------------------
cmake --build build_shared --config=Debug






echo --------------------------------------------------------
echo -- Configuring PHP-CPP static library
echo --------------------------------------------------------

cmake -S . -B build_static ^
-G "Visual Studio 15 2017" -T host=x64 -A x64 ^
-DPHPCPP_BUILD_SHARED=FALSE ^
-DPHP_DEV_DIR=%PHP_DEV_DIR% ^
-DPHPCPP_COMPILE_TS=%PHPCPP_COMPILE_TS% ^
-DPHPCPP_ARCH=%PHPCPP_ARCH%


timeout 3 > NUL
echo --------------------------------------------------------
echo -- Building Static Library - Release 
echo --------------------------------------------------------
cmake --build build_static --config=Release


timeout 3 > NUL
echo --------------------------------------------------------
echo -- Building Static Library - Debug 
echo --------------------------------------------------------
cmake --build build_static --config=Debug