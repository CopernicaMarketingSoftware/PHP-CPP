@echo off

REM
REM Test the extension
REM
REM Author: jefrisibarani@gmail.com
REM

set argC=0
for %%x in (%*) do Set /A argC+=1

if %argC% NEQ 3 (
   echo usage       : test_extension sample_name build_mode lib_mode
   echo sample_name : sample name, same with php script file name and extension name
   echo build_mode  : debug or release
   echo lib_mode    : shared or static
   echo example     : test_extension functionwithparameters release shared
   echo             : test_extension functionwithparameters release static
   exit /b
)

set arg_sample=%1
set arg_build=%2
set arg_lib=%3

set drive=%~d0%
set workdir=%~p0%
set prj_root=%drive%%workdir%
set php_root=d:\wampje\php\php.7.4.30
set phpcpp_compiled_root=%prj_root%\_output

rem sample name
set sample=%arg_sample%

rem debug or release
set build_mode=debug
if "%arg_build%"=="release" (
   set build_mode=release
)

rem static or shared
set lib_mode=shared
if "%arg_lib%"=="static" (
   set lib_mode=static
)


if "%lib_mode%"=="shared" (
   copy %phpcpp_compiled_root%\lib_shared\%build_mode%\phpcpp.dll %php_root% /y
)

%php_root%\php.exe -dextension=%prj_root%_output\ext_%lib_mode%\%build_mode%\%sample%.dll %prj_root%_output\ext_%lib_mode%\%build_mode%\%sample%.php

if "%lib_mode%"=="shared" (
   del %php_root%\phpcpp.dll
)