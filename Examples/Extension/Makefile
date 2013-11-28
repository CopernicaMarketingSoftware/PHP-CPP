CPP             = g++
RM              = rm -f
CPP_FLAGS       = -Wall -c -I. -O2 -std=c++11

LD              = g++
LD_FLAGS        = -Wall -shared -O2 
RESULT          = extension.so

SOURCES			= $(wildcard *.cpp)
OBJECTS         = $(SOURCES:%.cpp=%.o)

all:	${OBJECTS} ${RESULT}

${RESULT}: ${OBJECTS}
		${LD} ${LD_FLAGS} -o $@ ${OBJECTS} -lphpcpp

clean:
		${RM} *.obj *~* ${OBJECTS} ${RESULT}

${OBJECTS}: 
		${CPP} ${CPP_FLAGS} -fpic -o $@ ${@:%.o=%.cpp}
