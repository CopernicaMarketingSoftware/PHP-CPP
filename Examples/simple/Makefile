CPP             = g++
RM              = rm -f
CPP_FLAGS       = -Wall -c -I. -I/home/work/include/ -O2 -std=c++11

PREFIX			= /home/work/

#Edit these lines to correspond with your own directories
LIBRARY_DIR		= ${PREFIX}/local/php/lib/php/extensions/no-debug-non-zts-20090626/

PHP_CONFIG_DIR	= /home/work/local/php/lib/

LD              = g++
LD_FLAGS        = -Wall -Wl,-rpath,/home/work/lib -shared -O2 -L/home/work/lib
RESULT          = simple.so

PHPINIFILE		= 30-phpcpp.ini

SOURCES			= $(wildcard *.cpp)
OBJECTS         = $(SOURCES:%.cpp=%.o)

all:	${OBJECTS} ${RESULT}

${RESULT}: ${OBJECTS}
		${LD} ${LD_FLAGS} -o $@ ${OBJECTS} -lphpcpp

clean:
		${RM} *.obj *~* ${OBJECTS} ${RESULT}

${OBJECTS}: 
		${CPP} ${CPP_FLAGS} -fpic -o $@ ${@:%.o=%.cpp}

install:
		cp -f ${RESULT} ${LIBRARY_DIR}
		cp -f ${PHPINIFILE}	${PHP_CONFIG_DIR}
