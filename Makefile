PREFIX		=	/usr
INCLUDE_DIR = 	${PREFIX}/include
LIBRARY_DIR	=	${PREFIX}/lib

all:
		cd src; $(MAKE) -j

tests:
		cd tests; $(MAKE)

clean:
		cd src; $(MAKE) clean
#		cd tests; $(MAKE) clean

install:
		mkdir -p ${INCLUDE_DIR}/phpcpp
		mkdir -p ${LIBRARY_DIR}
		cp -f phpcpp.h ${INCLUDE_DIR}
		cp -f include/*.h ${INCLUDE_DIR}/phpcpp
		cp -f src/libphpcpp.so ${LIBRARY_DIR}

