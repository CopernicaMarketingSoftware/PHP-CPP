CPP             = g++
RM              = rm -f
PHP_DIR         = /usr/include/php5
CPP_FLAGS       = -g -Wno-write-strings  -c -I. -I${PHP_DIR} -I${PHP_DIR}/main -I${PHP_DIR}/ext -I${PHP_DIR}/Zend -I${PHP_DIR}/TSRM -O2 -std=c++11

LD              = g++
LD_FLAGS        = -Wall -shared -O2 
RESULT          = libphpcpp.so

SOURCES			= $(wildcard *.cpp)
OBJECTS         = $(SOURCES:%.cpp=%.o)

all:	${OBJECTS} ${RESULT}

${RESULT}: ${OBJECTS}
		${LD} ${LD_FLAGS} -o $@ ${OBJECTS}

clean:
		${RM} *.obj *~* ${OBJECTS} ${RESULT}

${OBJECTS}: 
		${CPP} ${CPP_FLAGS} -fpic -o $@ ${@:%.o=%.cpp}

