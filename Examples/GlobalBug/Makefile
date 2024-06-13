#   The name of the project, which is also how the .cpp and .php files are called,
#   and how the resulting .so and .ini file will be called.
NAME			= globalbug

#   Where to find PHP-CPP relative to this directory
PHPCPPROOT		= ../..

#   Where the resulting files will be stored
LIBRARY_DIR		= $(shell ${PHP_CONFIG} --extension-dir)
PHP_CONFIG_DIR	= /etc/php5/cli/conf.d


#   The compiler and its flags
CXX				= g++
CXX_FLAGS       = 	-c -I. -I ${PHPCPPROOT} -Og -g -std=c++11 \
					-Wall -Wextra -Wno-unused-parameter -Wno-format

#   The linker and its flags
LINKER			= g++
LINKER_FLAGS	= -Wall -shared -O2  -L ${PHPCPPROOT}

#   In case the programs aren't on the default location, change these variables
RM				= rm -rf
CP				= cp -f

#   Get all files that have to be compiled
SOURCES			= $(wildcard *.cpp)
HEADERS			= $(wildcard *.h)
OBJECTS         = $(SOURCES:%.cpp=%.o)

#   Targets that don't create files with the same name
.PHONY: all clean run install

all: ${NAME}.so

${NAME}.so: ${OBJECTS}
	${LINKER} ${LINKER_FLAGS} -o $@ ${OBJECTS} -lphpcpp

${OBJECTS}: %.o: %.cpp ${HEADERS}
	${CXX} ${CXX_FLAGS} -fpic -o $@ $<


clean:
	${RM} *.obj *~* "${OBJECTS}" "${NAME}.so"

#   Since the phpcpp.so file is (during development) in an unusual directory,
#   we have to specify where we can find it.
run: export LD_LIBRARY_PATH = ${PHPCPPROOT}
run: ${NAME}.so
	@# -n: ignore global php.ini file
	@php -n -d extension="${NAME}.so" -d extension_dir=. "${NAME}.php"

install:
	${CP} ${NAME}.so ${LIBRARY_DIR}
	echo "extension=${NAME}.so" > ${PHP_CONFIG_DIR}/30-${NAME}.ini
