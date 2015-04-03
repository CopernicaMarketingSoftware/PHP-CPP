CPP             = g++
RM              = rm -f
CPP_FLAGS       = -Wall -c -I. -O2 -std=c++11

PREFIX			= /usr
#Edit these lines to correspond with your own directories
LIBRARY_DIR		= $(shell ${PHP_CONFIG} --extension-dir)
PHP_CONFIG_DIR	= /etc/php5/cli/conf.d

LD              = g++
LD_FLAGS        = -Wall -shared -O2 
RESULT          = extension.so

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
