CPP             = g++
RM              = rm -f
CPP_FLAGS       = -Wall -c -I. -O2 -std=c++11

PHP_CONFIG      = $(shell which php-config)
LIBRARY_DIR		= $(shell ${PHP_CONFIG} --extension-dir)
PHP_CONFIG_DIR	= $(shell ${PHP_CONFIG} --ini-dir)

LD              = g++
LD_FLAGS        = -Wall -shared -O2 
RESULT          = mytestext.so

PHPINIFILE		= mytestext.ini

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
		cp -f ${RESULT} ${LIBRARY_DIR}/
		cp -f ${PHPINIFILE}	${PHP_CONFIG_DIR}/

uninstall:
		rm ${LIBRARY_DIR}/${RESULT}
		rm ${PHP_CONFIG_DIR}/${PHPINIFILE}