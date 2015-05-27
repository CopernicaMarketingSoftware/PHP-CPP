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

PHP_CONFIG				=	php-config


#
#   PHP binary file
#
#   The path to the executable PHP binary file.
#   Need to run tests.
#   You can see the command "whereis -b php"
#   Usually /usr/bin/php
#

PHP_BIN					=	$(shell ${PHP_CONFIG} --php-binary)


#
#   Installation directory
#
#   When you install the PHP-CPP library, it will place a number of C++ *.h
#   header files in your system include directory, and a libphpcpp.so shared
#   library file in your system libraries directory. Most users set this to
#   the regular /usr/include and /usr/lib directories, or /usr/local/include
#   and /usr/local/lib. You can of course change it to whatever suits you best
#

INSTALL_PREFIX			=	/usr
INSTALL_HEADERS			=	${INSTALL_PREFIX}/include

MACHINE := $(shell uname -m)
ifeq ($(MACHINE), x86_64)
INSTALL_LIB				=	${INSTALL_PREFIX}/lib64
endif
ifeq ($(MACHINE), i686)
INSTALL_LIB				=	${INSTALL_PREFIX}/lib
endif


#
#   SONAME and version
#
#   When ABI changes, soname and minor version of the library should be raised.
#   Otherwise only release verions changes. (version is MAJOR.MINOR.RELEASE)
#

SONAME					=	1.4
VERSION					=	1.4.0


#
#   Name of the target library name and config-generator
#
#   The PHP-CPP library will be installed on your system as libphpcpp.so.
#   This is a brilliant name. If you want to use a different name for it,
#   you can change that here.
#

PHP_SHARED_LIBRARY		=	libphpcpp.so.$(VERSION)
PHP_STATIC_LIBRARY		=	libphpcpp.a.$(VERSION)
HHVM_SHARED_LIBRARY		=	libhhvmcpp.so.$(VERSION)
HHVM_STATIC_LIBRARY		=	libhhvmcpp.a.$(VERSION)


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

COMPILER_FLAGS			=	-Wall -c -std=c++11 -fvisibility=hidden -DBUILDING_PHPCPP
SHARED_COMPILER_FLAGS	=	-fpic
STATIC_COMPILER_FLAGS	=
PHP_COMPILER_FLAGS		=	${COMPILER_FLAGS} `${PHP_CONFIG} --includes`
HHVM_COMPILER_FLAGS		=	${COMPILER_FLAGS}

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
PHP_LINKER_FLAGS		=	${LINKER_FLAGS} `${PHP_CONFIG} --ldflags`
HHVM_LINKER_FLAGS		=	${LINKER_FLAGS}


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
#   common/, zend/ and hhvm/ directories for all *.cpp files. No changes are
#   probably necessary here
#

COMMON_SOURCES			=	$(wildcard common/*.cpp)
PHP_SOURCES				=	$(wildcard zend/*.cpp)
HHVM_SOURCES			=	$(wildcard hhvm/*.cpp)

#
#   The object files
#
#   The intermediate object files are generated by the compiler right before
#   the linker turns all these object files into the libphpcpp.so and
#   libhhvmcpp.so shared libraries. We also use a Makefile function here that
#   takes all source files.
#

COMMON_SHARED_OBJECTS	=	$(COMMON_SOURCES:%.cpp=shared/%.o)
PHP_SHARED_OBJECTS		=	$(PHP_SOURCES:%.cpp=shared/%.o)
HHVM_SHARED_OBJECTS		=	$(HHVM_SOURCES:%.cpp=shared/%.o)
COMMON_STATIC_OBJECTS	=	$(COMMON_SOURCES:%.cpp=static/%.o)
PHP_STATIC_OBJECTS		=	$(PHP_SOURCES:%.cpp=static/%.o)
HHVM_STATIC_OBJECTS		=	$(HHVM_SOURCES:%.cpp=static/%.o)


#
#   End of the variables section. Here starts the list of instructions and
#   dependencies that are used by the compiler.
#

all: COMPILER_FLAGS 	+=	-g
all: LINKER_FLAGS		+=  -g
all: phpcpp

release: COMPILER_FLAGS +=	-O2
release: LINKER_FLAGS	+=  -O2
release: phpcpp

phpcpp: ${PHP_SHARED_LIBRARY} ${PHP_STATIC_LIBRARY}
	@echo
	@echo "Build complete."

hhvmcpp: ${HHVM_SHARED_LIBRARY} ${PHP_STATIC_LIBRARY}
	@echo
	@echo "Build complete."

${PHP_SHARED_LIBRARY}: shared_directories ${COMMON_SHARED_OBJECTS} ${PHP_SHARED_OBJECTS}
	${LINKER} ${PHP_LINKER_FLAGS} -Wl,-soname,libphpcpp.so.$(SONAME) -o $@ ${COMMON_SHARED_OBJECTS} ${PHP_SHARED_OBJECTS}

${PHP_STATIC_LIBRARY}: static_directories ${COMMON_STATIC_OBJECTS} ${PHP_STATIC_OBJECTS}
	${ARCHIVER} $@ ${COMMON_STATIC_OBJECTS} ${PHP_STATIC_OBJECTS}

${HHVM_SHARED_LIBRARY}: shared_directories ${COMMON_SHARED_OBJECTS} ${HHVM_SHARED_OBJECTS}
	${LINKER} ${HHVM_LINKER_FLAGS} -Wl,-soname,libhhvmcpp.so.$(SONAME) -o $@ ${COMMON_SHARED_OBJECTS} ${HHVM_SHARED_OBJECTS}

${HHVM_STATIC_LIBRARY}: static_directories ${COMMON_STATIC_OBJECTS} ${HHVM_STATIC_OBJECTS}
	${ARCHIVER} $@ ${COMMON_STATIC_OBJECTS} ${HHVM_STATIC_OBJECTS}

shared_directories:
	${MKDIR} shared/common
	${MKDIR} shared/zend
	${MKDIR} shared/hhvm

static_directories:
	${MKDIR} static/common
	${MKDIR} static/zend
	${MKDIR} static/hhvm

clean:
	${RM} shared ${PHP_SHARED_LIBRARY} ${HHVM_SHARED_LIBRARY}
	${RM} static ${PHP_STATIC_LIBRARY} ${HHVM_STATIC_LIBRARY}
	find -name *.o | xargs ${RM}

${COMMON_SHARED_OBJECTS}:
	${COMPILER} ${COMPILER_FLAGS} ${SHARED_COMPILER_FLAGS} -o $@ ${@:shared/%.o=%.cpp}

${COMMON_STATIC_OBJECTS}:
	${COMPILER} ${COMPILER_FLAGS} ${STATIC_COMPILER_FLAGS} -o $@ ${@:static/%.o=%.cpp}

${PHP_SHARED_OBJECTS}:
	${COMPILER} ${PHP_COMPILER_FLAGS} ${SHARED_COMPILER_FLAGS} -o $@ ${@:shared/%.o=%.cpp}

${PHP_STATIC_OBJECTS}:
	${COMPILER} ${PHP_COMPILER_FLAGS} ${STATIC_COMPILER_FLAGS} -o $@ ${@:static/%.o=%.cpp}

${HHVM_SHARED_OBJECTS}:
	${COMPILER} ${HHVM_COMPILER_FLAGS} ${SHARED_COMPILER_FLAGS} -o $@ ${@:shared/%.o=%.cpp}

${HHVM_STATIC_OBJECTS}:
	${COMPILER} ${HHVM_COMPILER_FLAGS} ${STATIC_COMPILER_FLAGS} -o $@ ${@:static/%.o=%.cpp}

# The if statements below must be seen as single line by make

install:
	${MKDIR} ${INSTALL_HEADERS}/phpcpp
	${CP} phpcpp.h ${INSTALL_HEADERS}
	${CP} include/*.h ${INSTALL_HEADERS}/phpcpp
	if [ -e ${PHP_SHARED_LIBRARY} ]; then \
		${CP} ${PHP_SHARED_LIBRARY} ${INSTALL_LIB}/; \
		${LN} ${INSTALL_LIB}/${PHP_SHARED_LIBRARY} ${INSTALL_LIB}/libphpcpp.so.$(SONAME); \
		${LN} ${INSTALL_LIB}/${PHP_SHARED_LIBRARY} ${INSTALL_LIB}/libphpcpp.so; \
	fi
	if [ -e ${PHP_STATIC_LIBRARY} ]; then ${CP} ${PHP_STATIC_LIBRARY} ${INSTALL_LIB}/; \
		${LN} ${INSTALL_LIB}/${PHP_STATIC_LIBRARY} ${INSTALL_LIB}/libphpcpp.a; \
	fi
	if [ -e ${HHVM_SHARED_LIBRARY} ]; then \
		${CP} ${HHVM_SHARED_LIBRARY} ${INSTALL_LIB}/; \
		${LN} ${INSTALL_LIB}/${HHVM_SHARED_LIBRARY} ${INSTALL_LIB}/libhhvmcpp.so.$(SONAME);\
		${LN} ${INSTALL_LIB}/${HHVM_SHARED_LIBRARY} ${INSTALL_LIB}/libhhvmcpp.so; \
	fi
	if [ -e ${HHVM_STATIC_LIBRARY} ]; then \
		${CP} ${HHVM_STATIC_LIBRARY} ${INSTALL_LIB}/; \
		${LN} ${INSTALL_LIB}/${HHVM_STATIC_LIBRARY} ${INSTALL_LIB}/libhhvmcpp.a; \
	fi

test:
	mkdir -p tests/include/zts/phpcpp
	cd tests && ./test.sh -p "${PHP_BIN}"

