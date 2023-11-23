cmake_minimum_required(VERSION 3.11-3.18)

set(PHPCPP_VERSION 2.3.5)
project(phpcpp VERSION ${PHPCPP_VERSION} LANGUAGES CXX)


# -----------------------------------------------------------------------------
#                       Configurable options
#
# Uncomment and set the value, or set directly with cmake
# -----------------------------------------------------------------------------
#set(PHPCPP_BUILD_SHARED   TRUE)  # Build static or shared library
#set(PHPCPP_BUILD_EXAMPLES TRUE)  # Build example extensions
#set(PHPCPP_INSTALL        TRUE)  # Generate install target

# On Linux, uncomment to set different php-config filename
#set(PHP_CONFIG_FILENAME "php-config")

# Options for building on Windows with MSVC
# -----------------------------------------------------------------------------
# PHP devel pack dir, download from https://windows.php.net/download/
#set(PHP_DEV_DIR         "d:/dev/cpp_php/ts/php-7.4.30-devel-vc15-x64")
# Set to TRUE if PHP was compiled with thread safe
#set(PHPCPP_COMPILE_TS    TRUE)
# Set to "x86_64" if architecture and PHP was compiled under x64 or "x86" for 32 bit
#set(PHPCPP_ARCH          "x86_64")



# -----------------------------------------------------------------------------
# Default values
# -----------------------------------------------------------------------------
# Build static or shared library
if(NOT DEFINED PHPCPP_BUILD_SHARED)
   set(PHPCPP_BUILD_SHARED   TRUE)
endif()

# Build example extensions
if(NOT DEFINED PHPCPP_BUILD_EXAMPLES)
   set(PHPCPP_BUILD_EXAMPLES TRUE)
endif()

# Generate library install target
if(NOT DEFINED PHPCPP_INSTALL)
   set(PHPCPP_INSTALL        TRUE)
endif()


if(PHPCPP_BUILD_SHARED)
   message(STATUS "Build PHP-CPP ${PHPCPP_VERSION} shared library...")
else()
   message(STATUS "Build PHP-CPP ${PHPCPP_VERSION} static library...")
endif()


if(WIN32)
   # Windows build options
   if(NOT DEFINED PHP_DEV_DIR)
      message(FATAL_ERROR   "PHP development dir variable PHP_DEV_DIR not defined!")
   endif()

   if(NOT DEFINED PHPCPP_COMPILE_TS)
      message(FATAL_ERROR   "PHP threading(TS/NTS) variable PHPCPP_COMPILE_TS not defined!")
   endif()

   if(NOT DEFINED PHPCPP_ARCH)
      message(FATAL_ERROR   "PHP architecture variable PHPCPP_ARCH not defined!")
   endif()

   # PHP library dir
   set(PHP_LIB_DIR          "${PHP_DEV_DIR}/lib")
   # PHP library file name
   set(PHP_LIB_NAME         "php7ts")
   if(NOT PHPCPP_COMPILE_TS)
      set(PHP_LIB_NAME      "php7")
   endif()

   message(STATUS "PHP_LIB_DIR           ${PHP_LIB_DIR}")
   message(STATUS "PHPCPP_ARCH           ${PHPCPP_ARCH}")
   message(STATUS "PHPCPP_COMPILE_TS     ${PHPCPP_COMPILE_TS}")

else()
   # Non windows, try detect php with php-config
   include(cmake/utils.cmake)

   if(NOT DEFINED PHP_CONFIG_FILENAME)
      find_php_dev(php-config) # use default php-config
   else()
      find_php_dev(${PHP_CONFIG_FILENAME})
   endif()

   if(NOT PHP_DEV_FOUND)
      message(FATAL_ERROR "PHP development not found!")
   endif()

   if(PHP_VERSION_NUMBER LESS 70000)
      message(FATAL_ERROR "PHP-CPP works only with PHP 7")
   endif()

   message(STATUS "PHP_VERSION           ${PHP_VERSION}")
   include(GNUInstallDirs)
endif()


# Miscs php-cpp variables
# -----------------------------------------------------------------------------
set(PHPCPP_SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR} )
message(STATUS "PHPCPP_SOURCE_DIR     ${PHPCPP_SOURCE_DIR}")

if(PHPCPP_SOURCE_DIR STREQUAL CMAKE_SOURCE_DIR)
   # we are master project
   set(PHPCPP_OUTPUT_DIR ${PHPCPP_SOURCE_DIR}/_output)
else()
   set(PHPCPP_OUTPUT_DIR ${CMAKE_SOURCE_DIR}/_output)
endif()


# Include headers and sources list
include(cmake/sources.cmake)


# phpcpp-common
# -----------------------------------------------------------------------------
add_library(phpcpp-common STATIC)

target_sources(phpcpp-common
   PUBLIC  ${PHPCPP_COMMON_HEADERS}
   PRIVATE ${PHPCPP_COMMON_SOURCES} )

if(WIN32)
   target_include_directories(phpcpp-common
      PUBLIC ${PHP_DEV_DIR}/include
      PUBLIC ${PHP_DEV_DIR}/include/main
      PUBLIC ${PHP_DEV_DIR}/include/Zend
      PUBLIC ${PHP_DEV_DIR}/include/TSRM
      PUBLIC ${PHP_DEV_DIR}/include/ext )

   #target_compile_definitions(phpcpp-common
   #   PUBLIC $<$<CONFIG:Debug>:ZEND_DEBUG=1>$<$<CONFIG:Release>:ZEND_DEBUG=0> )
   # Always use PHP release build
   target_compile_definitions(phpcpp-common PUBLIC ZEND_DEBUG=0 )

   if(PHPCPP_COMPILE_TS)
      target_compile_definitions(phpcpp-common PUBLIC ZTS )
   endif()

   if("x86" STREQUAL ${PHPCPP_ARCH})
      target_compile_definitions(phpcpp-common PUBLIC _USE_32BIT_TIME_T )
   endif()

   target_compile_definitions(phpcpp-common
      PUBLIC PHP_WIN32
      PUBLIC ZEND_WIN32
      PUBLIC ZEND_WIN32_KEEP_INLINE )

   if(MSVC)
      target_compile_definitions(phpcpp-common
         PUBLIC _MSC_STDINT_H_
         PUBLIC _CRT_SECURE_NO_WARNINGS
         PRIVATE W3 )
   endif()

else() # not win32
   target_compile_definitions(phpcpp-common PRIVATE BUILDING_PHPCPP)
   target_compile_options(phpcpp-common PRIVATE -fpic -c -fvisibility=hidden -Wno-write-strings -MD -Wall )
   target_include_directories(phpcpp-common PUBLIC ${PHP_INCLUDE_DIRS})
endif()

target_compile_features(phpcpp-common PRIVATE cxx_std_11)


# phpcpp library
# -----------------------------------------------------------------------------
if(PHPCPP_BUILD_SHARED)
   add_library(phpcpp SHARED)
   if(NOT MSVC)
      target_link_options(phpcpp PRIVATE -shared ${PHP_LDFLAGS})
   endif()
else()
   add_library(phpcpp STATIC)
endif()

target_link_libraries(phpcpp PRIVATE phpcpp-common)

if(WIN32)
   if(PHPCPP_BUILD_SHARED)
      target_compile_definitions(phpcpp PRIVATE BUILDING_PHPCPP)
   endif()
   target_link_directories(phpcpp PUBLIC ${PHP_LIB_DIR})
   target_link_libraries(phpcpp PUBLIC ${PHP_LIB_NAME} )
else()
   target_compile_definitions(phpcpp PRIVATE BUILDING_PHPCPP)
   target_compile_options(phpcpp PRIVATE -fpic -c -fvisibility=hidden -Wno-write-strings -MD -Wall )
endif()

target_sources(phpcpp
   PRIVATE ${PHPCPP_HEADERS_FILES}
   PRIVATE ${PHPCPP_ZEND_HEADERS}
   PRIVATE ${PHPCPP_ZEND_SOURCES} )

target_compile_features(phpcpp PRIVATE cxx_std_11 )


# Prepare phpcpp header files for dependend projects and building examples
# publish PHPCPP_INCLUDE_DIR variable to outside world
# -----------------------------------------------------------------------------
set(PHPCPP_INCLUDE_DIR "${CMAKE_BINARY_DIR}/phpcpp_include")
file(COPY phpcpp.h DESTINATION ${PHPCPP_INCLUDE_DIR})
file(COPY include/ DESTINATION ${PHPCPP_INCLUDE_DIR}/phpcpp)

if(NOT PHPCPP_SOURCE_DIR STREQUAL CMAKE_SOURCE_DIR)
   set(PHPCPP_INCLUDE_DIR ${PHPCPP_INCLUDE_DIR} PARENT_SCOPE)
endif()
message(STATUS "PHPCPP_INCLUDE_DIR    ${PHPCPP_INCLUDE_DIR}")


# Build examples
#-----------------------------------------------------------------------------
if(PHPCPP_BUILD_EXAMPLES)
   message(STATUS "Generating examples")
   add_subdirectory(Examples)
endif()


# Install
# -----------------------------------------------------------------------------
if(PHPCPP_INSTALL)
   message(STATUS "Generating install")
   install(FILES phpcpp.h DESTINATION "${CMAKE_INSTALL_INCLUDEDIR}/")
   install(DIRECTORY include/ DESTINATION "${CMAKE_INSTALL_INCLUDEDIR}/phpcpp/")

   install( TARGETS phpcpp-common phpcpp
      LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR} )
endif()


# Copy binaries, resources to _output folder
# -----------------------------------------------------------------------------
set(OUTPUT_LIBDIR "${PHPCPP_OUTPUT_DIR}/lib_static")
if (PHPCPP_BUILD_SHARED)
  set(OUTPUT_LIBDIR "${PHPCPP_OUTPUT_DIR}/lib_shared")
endif()

# Copy build output dir to _output dir
add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
   COMMAND ${CMAKE_COMMAND} -E make_directory ${OUTPUT_LIBDIR}/$<$<CONFIG:Debug>:debug>$<$<CONFIG:Release>:release>
   COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_FILE:phpcpp> ${OUTPUT_LIBDIR}/$<$<CONFIG:Debug>:debug>$<$<CONFIG:Release>:release>
   COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_FILE:phpcpp-common> ${OUTPUT_LIBDIR}/$<$<CONFIG:Debug>:debug>$<$<CONFIG:Release>:release> )

if(WIN32 AND PHPCPP_BUILD_SHARED)
   add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
      COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_LINKER_FILE:phpcpp> ${OUTPUT_LIBDIR}/$<$<CONFIG:Debug>:debug>$<$<CONFIG:Release>:release>)
endif()

# Copy headers to _output dir
add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
  COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_CURRENT_SOURCE_DIR}/include ${PHPCPP_OUTPUT_DIR}/include/phpcpp )

# Copy phpcpp.h to _output dir
add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
  COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_CURRENT_SOURCE_DIR}/phpcpp.h ${PHPCPP_OUTPUT_DIR}/include )