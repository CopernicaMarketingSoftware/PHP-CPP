CMAKE_MINIMUM_REQUIRED(VERSION 3.0)

PROJECT(phpcpp)

# Use CMake File with following options
# PHPCPP_COMPILE_TS     Set flag if PHP was compiled with thread safe
# PHPCPP_MSVCRT_STATIC  Set flag if you want to compile the MSVC runtime as a static library
# PHPCPP_ARCH           Set to "x86_64" if architecture and PHP was compiled under x64 or "x86" for 32 bit
# PHPCPP_PHP_PATH       The path to PHP sources

LIST(APPEND CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR})

IF(CMAKE_BUILD_TYPE MATCHES DEBUG)
    ADD_DEFINITIONS(-DZEND_DEBUG=1)
ELSE()
    ADD_DEFINITIONS(-DZEND_DEBUG=0)
ENDIF()

IF (PHPCPP_COMPILE_TS)
  ADD_DEFINITIONS(-DZTS)
ENDIF()

ADD_DEFINITIONS(-D_MSC_STDINT_H_)
ADD_DEFINITIONS(-D_CRT_SECURE_NO_WARNINGS)
ADD_DEFINITIONS(-DBUILDING_PHPCPP)
ADD_DEFINITIONS(-DZEND_WIN32_KEEP_INLINE)

IF (MSVC)
  # Should compile MSVC runtime static or dll
  IF (PHPCPP_MSVCRT_STATIC)
    MESSAGE("Compile PHPCPP with static runtime")

    SET(CMAKE_CXX_FLAGS_RELEASE         "${CMAKE_CXX_FLAGS_RELEASE} /MT")
    SET(CMAKE_CXX_FLAGS_DEBUG           "${CMAKE_CXX_FLAGS_DEBUG} /MTd")
    SET(CMAKE_CXX_FLAGS_RELWITHDEBINFO  "${CMAKE_CXX_FLAGS_RELWITHDEBINFO} /MT")

  ELSE()
    MESSAGE("Compile PHPCPP with dynamic runtime")

    SET(CMAKE_CXX_FLAGS_RELEASE         "${CMAKE_CXX_FLAGS_RELEASE} /MD")
    SET(CMAKE_CXX_FLAGS_DEBUG           "${CMAKE_CXX_FLAGS_DEBUG} /MDd")
    SET(CMAKE_CXX_FLAGS_RELWITHDEBINFO  "${CMAKE_CXX_FLAGS_RELWITHDEBINFO} /MD")

  ENDIF()

  IF ("${PHPCPP_ARCH}" STREQUAL "x86_64")
    ADD_DEFINITIONS(-DPHP_WIN32 -DZEND_WIN32)
  ELSE()
    ADD_DEFINITIONS(-DPHP_WIN32 -DZEND_WIN32 -D_USE_32BIT_TIME_T)
  ENDIF()

  SET(ADDITIONAL_COMPILER_FLAGS "/W3")
  SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${ADDITIONAL_COMPILER_FLAGS}")
ENDIF ()

INCLUDE_DIRECTORIES(
  "${PHPCPP_PHP_PATH}/"
  "${PHPCPP_PHP_PATH}/main"
  "${PHPCPP_PHP_PATH}/Zend"
  "${PHPCPP_PHP_PATH}/TSRM"
  "${PHPCPP_PHP_PATH}/ext"
  "${PHPCPP_PHP_PATH}/build/main"
  "${PHPCPP_PHP_PATH}/build/Zend"
)
SET(PHP_LIBS "php")
SET(PHP_LIB_PATH "")

IF ("${PHPCPP_ARCH}" STREQUAL "x86_64")
  SET(PHP_LIB_PATH "x64/")
ENDIF()

IF(CMAKE_BUILD_TYPE MATCHES DEBUG)
  IF (PHPCPP_COMPILE_TS)
    SET(PHP_LIBS      ${PHP_LIBS} "php7ts_debug")
    SET(PHP_LIB_PATH  "${PHP_LIB_PATH}Debug_TS")
  ELSE()
    SET(PHP_LIBS      ${PHP_LIBS} "php7_debug")
    SET(PHP_LIB_PATH  "${PHP_LIB_PATH}Debug")
  ENDIF()
ELSE()
  IF (PHPCPP_COMPILE_TS)
    SET(PHP_LIBS      ${PHP_LIBS} "php7ts")
    SET(PHP_LIB_PATH  "${PHP_LIB_PATH}Release_TS")
  ELSE()
    SET(PHP_LIBS      ${PHP_LIBS} "php7")
    SET(PHP_LIB_PATH  "${PHP_LIB_PATH}Release")
  ENDIF()
ENDIF()

SET(PHPCPP_COMMON_SOURCES
  common/modifiers.cpp
  common/streambuf.cpp
)

SET(PHPCPP_COMMON_HEADERS
  common/extensionbase.h
  common/includes.h
  common/streambuf.h
)

ADD_LIBRARY(phpcpp-common STATIC
  ${PHPCPP_COMMON_HEADERS}
  ${PHPCPP_COMMON_SOURCES}
)

set_target_properties(phpcpp-common
  PROPERTIES
  ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}"
  LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}"
  RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}"
)

SET(PHPCPP_ZEND_SOURCES
  zend/base.cpp
  zend/callable.cpp
  zend/classbase.cpp
  zend/classimpl.cpp
  zend/constant.cpp
  zend/constantfuncs.cpp
  zend/eval.cpp
  zend/exception_handler.cpp
  zend/exists.cpp
  zend/extension.cpp
  zend/extensionimpl.cpp
  # zend/fatalerror.cpp
  zend/file.cpp
  zend/function.cpp
  zend/functor.cpp
  zend/global.cpp
  zend/globals.cpp
  zend/hashmember.cpp
  zend/ini.cpp
  zend/inivalue.cpp
  zend/iteratorimpl.cpp
  zend/members.cpp
  zend/module.cpp
  zend/namespace.cpp
  zend/object.cpp
  zend/sapi.cpp
  zend/script.cpp
  zend/streambuf.cpp
  zend/streams.cpp
  zend/super.cpp
  zend/value.cpp
  zend/valueiterator.cpp
  zend/zendcallable.cpp
  zend/zval.cpp
)

SET(PHPCPP_ZEND_HEADERS
  zend/arithmetic.h
  zend/boolmember.h
  zend/callable.h
  zend/classimpl.h
  zend/compileroptions.h
  zend/constantimpl.h
  zend/delayedfree.h
  zend/executestate.h
  zend/extensionimpl.h
  zend/extensionpath.h
  zend/floatmember.h
  zend/functor.h
  zend/hashiterator.h
  zend/includes.h
  zend/init.h
  zend/invaliditerator.h
  zend/iteratorimpl.h
  zend/lowercase.h
  zend/member.h
  zend/method.h
  zend/module.h
  zend/nativefunction.h
  zend/notimplemented.h
  zend/nullmember.h
  zend/numericmember.h
  zend/objectimpl.h
  zend/opcodes.h
  # zend/origexception.h
  zend/parametersimpl.h
  zend/property.h
  zend/string.h
  zend/stringmember.h
  zend/symbol.h
  zend/symbol.h
  zend/traverseiterator.h
  zend/valueiteratorimpl.h
)

SET(PHPCPP_HEADERS_INCLUDE
  include/argument.h
  include/array.h
  include/arrayaccess.h
  include/base.h
  include/byref.h
  include/byval.h
  include/call.h
  include/class.h
  include/classbase.h
  include/classtype.h
  include/constant.h
  include/countable.h
  include/deprecated.h
  include/error.h
  include/exception.h
  include/extension.h
  # include/fatalerror.h
  include/file.h
  include/function.h
  include/global.h
  include/globals.h
  include/hashmember.h
  include/hashparent.h
  include/ini.h
  include/inivalue.h
  include/interface.h
  include/iterator.h
  include/modifiers.h
  include/namespace.h
  include/noexcept.h
  include/object.h
  include/parameters.h
  include/platform.h
  include/script.h
  include/serializable.h
  include/streams.h
  include/super.h
  include/thread_local.h
  include/traversable.h
  include/type.h
  include/value.h
  include/valueiterator.h
  include/version.h
  include/visibility.h
  include/zendcallable.h
  include/zval.h
)

LINK_DIRECTORIES(${PHP_LIB_PATH})

IF (PHPCPP_SHARED)
  ADD_LIBRARY(phpcpp SHARED
    ${PHPCPP_HEADERS_INCLUDE}
    ${PHPCPP_ZEND_HEADERS}
    ${PHPCPP_ZEND_SOURCES}
    phpcpp.h
  )

  TARGET_LINK_LIBRARIES(phpcpp phpcpp-common ${PHP_LIBS})
ELSE()
  ADD_LIBRARY(phpcpp STATIC
    ${PHPCPP_HEADERS_INCLUDE}
    ${PHPCPP_ZEND_HEADERS}
    ${PHPCPP_ZEND_SOURCES}
    phpcpp.h
  )

  TARGET_LINK_LIBRARIES(phpcpp STATIC phpcpp-common ${PHP_LIBS})
ENDIF()

SET_TARGET_PROPERTIES(phpcpp
  PROPERTIES
  ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}"
  LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}"
  RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}"
)
