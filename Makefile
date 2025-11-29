#
#   PHP-CPP Makefile
#
#   This makefile has a user friendly order: the top part of this file contains
#   all variable settings that you may alter to suit your own system, while at
#   the bottom you will find instructions for the compiler in which you will
#   probably not have to make any changes
#

#
#   Php-config utility
#
#   PHP comes with a standard utility program called 'php-config'. This program
#   can be used to find out in which directories PHP is installed. Inside this
#   makefile this utility program is used to find include directories, shared
#   libraries and the path to the binary file. If your php-config is not
#   installed in the default directory, you can change that here.
#
#   If you use environment with multiple PHP versions (e.g. debian), you need
#   to specify name or path of php-config utility that matches targeted php
#   version. If you want to build for multiple PHP versions, then also consider
#   changing BUILD_DIR variable, so that builds for each version are separated
#

PHP_CONFIG			=	php-config
BUILD_DIR			=	build
UNAME 				:= 	$(shell uname)

#
#   If you do not want to run LDCONFIG, then set LDCONFIG to empty string.
#   alternatively you can provide another command to register libraries
#   after installation.
#

LDCONFIG	=	$(shell which ldconfig 2>/dev/null)

#
#   Installation directory
#
#   When you install the PHP-CPP library, it will place a number of C++ *.h
#   header files in your system include directory, and a libphpcpp.so shared
#   library file in your system libraries directory. Most users set this to
#   the regular /usr/include and /usr/lib directories, or /usr/local/include
#   and /usr/local/lib. You can of course change it to whatever suits you best
#

# Since OSX 10.10 Yosemite, /usr/include gives problem
# So, let's switch to /usr/local as default instead.
ifeq ($(UNAME), Darwin)
  INSTALL_PREFIX		=	/usr/local
else
  INSTALL_PREFIX		=	/usr
endif

INSTALL_HEADERS			=	${INSTALL_PREFIX}/include
INSTALL_LIB				=	${INSTALL_PREFIX}/lib


#
#   SONAME and version
#
#   When ABI changes, soname and minor version of the library should be raised.
#   Otherwise only release verions changes. (version is MAJOR.MINOR.RELEASE)
#

SONAME					=	2.4
VERSION					=	2.4.13


#
#   Name of the target library name and config-generator
#
#   The PHP-CPP library will be installed on your system as libphpcpp.so.
#   This is a brilliant name. If you want to use a different name for it,
#   you can change that here.
#

LIBRARY_NAME			=	libphpcpp

PHP_SHARED_LIBRARY		=	${LIBRARY_NAME}.so.$(VERSION)
PHP_STATIC_LIBRARY		=	${LIBRARY_NAME}.a.$(VERSION)


#
#   Compiler
#
#   By default, the GNU C++ compiler is used. If you want to use a different
#   compiler, you can change that here. You can change this for both the
#   compiler (the program that turns the c++ files into object files) and for
#   the linker (the program that links all object files into a single .so
#   library file. By default, g++ (the GNU C++ compiler) is used for both.
#

ifdef CXX
 COMPILER				=	${CXX}
 LINKER					=	${CXX}
else
 COMPILER				=	g++
 LINKER					=	g++
endif

ifdef AR
  ARCHIVER				=	${AR} rcs
else
  ARCHIVER				=	ar rcs
endif

#
#   Compiler flags
#
#   This variable holds the flags that are passed to the compiler. By default,
#   we include the -O2 flag. This flag tells the compiler to optimize the code,
#   but it makes debugging more difficult. So if you're debugging your application,
#   you probably want to remove this -O2 flag. At the same time, you can then
#   add the -g flag to instruct the compiler to include debug information in
#   the library (but this will make the final libphpcpp.so file much bigger, so
#   you want to leave that flag out on production servers).
#

COMPILER_FLAGS			=	-Wall -c -std=c++11 -fvisibility=hidden -DBUILDING_PHPCPP -Wno-write-strings -MD -fpic `${PHP_CONFIG} --includes`

#
#   Linker flags
#
#   Just like the compiler, the linker can have flags too. The default flag
#   is probably the only one you need.
#
#   Are you compiling on OSX? You may have to append the option "-undefined dynamic_lookup"
#   to the linker flags
#

LINKER_FLAGS			=	-shared
ifeq ($(UNAME), Darwin)
	LINKER_FLAGS		+=	-undefined dynamic_lookup
endif
PHP_LINKER_FLAGS		=	`${PHP_CONFIG} --ldflags` ${LINKER_FLAGS}

ifeq ($(UNAME), Darwin)
	LINKER_SONAME_OPTION	=	-install_name
else
	LINKER_SONAME_OPTION    =   -soname
endif

#
#   Command to remove files, copy files, link files and create directories.
#
#   I've never encountered a *nix environment in which these commands do not work.
#   So you can probably leave this as it is
#

RM						=	rm -fr
CP						=	cp -f
LN						=	ln -f -s
MKDIR					=	mkdir -p


#
#   The source files
#
#   For this we use a special Makefile function that automatically scans the
#   common/ and zend/ directories for all *.cpp files. No changes are
#   probably necessary here
#

COMMON_SOURCES			=	$(wildcard common/*.cpp)
PHP_SOURCES				=	$(wildcard zend/*.cpp)

#
#   The object files
#
#   The intermediate object files are generated by the compiler right before
#   the linker turns all these object files into the libphpcpp.so shared
#   library. We also use a Makefile function here that takes all source files.
#

COMMON_OBJECTS	=	$(COMMON_SOURCES:%.cpp=${BUILD_DIR}/%.o)
PHP_OBJECTS		=	$(PHP_SOURCES:%.cpp=${BUILD_DIR}/%.o)

#
#   Dependencies
#

DEPENDENCIES            =   $(wildcard ${BUILD_DIR}/common/*.d) $(wildcard ${BUILD_DIR}/zend/*.d)

#
#   End of the variables section. Here starts the list of instructions and
#   dependencies that are used by the compiler.
#

all: COMPILER_FLAGS 	+=	-g
all: LINKER_FLAGS		+=  -g
all: phpcpp

-include ${DEPENDENCIES}

release: COMPILER_FLAGS +=	-O2
release: LINKER_FLAGS	+=  -O2
release: phpcpp

phpcpp: ${BUILD_DIR}/${PHP_SHARED_LIBRARY} ${BUILD_DIR}/${PHP_STATIC_LIBRARY}
	@echo
	@echo "Build complete."

${BUILD_DIR}/${PHP_SHARED_LIBRARY}: build_directories ${COMMON_OBJECTS} ${PHP_OBJECTS}
	${LINKER} ${PHP_LINKER_FLAGS} -Wl,${LINKER_SONAME_OPTION},${LIBRARY_NAME}.so.$(SONAME) -o $@ ${COMMON_OBJECTS} ${PHP_OBJECTS}

${BUILD_DIR}/${PHP_STATIC_LIBRARY}: build_directories ${COMMON_OBJECTS} ${PHP_OBJECTS}
	${ARCHIVER} $@ ${COMMON_OBJECTS} ${PHP_OBJECTS}

build_directories:
	${MKDIR} ${BUILD_DIR}/common
	${MKDIR} ${BUILD_DIR}/zend

clean:
	${RM} ${BUILD_DIR}
	find -name *.o | xargs ${RM}
	find -name *.d | xargs ${RM}

${COMMON_OBJECTS}:
	${COMPILER} ${COMPILER_FLAGS} -o $@ ${@:${BUILD_DIR}/%.o=%.cpp}

${PHP_OBJECTS}:
	${COMPILER} ${COMPILER_FLAGS} -o $@ ${@:${BUILD_DIR}/%.o=%.cpp}


# The if statements below must be seen as single line by make

install:
	${MKDIR} ${INSTALL_HEADERS}/phpcpp
	${MKDIR} ${INSTALL_LIB}
	${CP} phpcpp.h ${INSTALL_HEADERS}
	${CP} include/*.h ${INSTALL_HEADERS}/phpcpp
	if [ -e ${BUILD_DIR}/${PHP_SHARED_LIBRARY} ]; then \
		${CP} ${BUILD_DIR}/${PHP_SHARED_LIBRARY} ${INSTALL_LIB}/ && \
		${LN} ${PHP_SHARED_LIBRARY} ${INSTALL_LIB}/${LIBRARY_NAME}.so.$(SONAME) && \
		${LN} ${PHP_SHARED_LIBRARY} ${INSTALL_LIB}/${LIBRARY_NAME}.so; \
	fi
	if [ -e ${BUILD_DIR}/${PHP_STATIC_LIBRARY} ]; then \
		${CP} ${BUILD_DIR}/${PHP_STATIC_LIBRARY} ${INSTALL_LIB}/ && \
		${LN} ${PHP_STATIC_LIBRARY} ${INSTALL_LIB}/${LIBRARY_NAME}.a; \
	fi
	${LDCONFIG}

uninstall:
	${RM} ${INSTALL_HEADERS}/phpcpp*
	${RM} ${INSTALL_LIB}/${LIBRARY_NAME}.*

