# Builds the test extension against the library in the repository root (run 'make' there first) and runs the test script.
ROOT	= $(abspath ../..)
LIBRARY	= $(wildcard $(ROOT)/libphpcpp.so.*.*.*)
SONAME	= $(shell readelf -d $(LIBRARY) | sed -n 's/.*soname: \[\(.*\)\]/\1/p')

# scratch directories inside the git-ignored build/ directory: the headers include each other as <phpcpp/...> and the
# library is looked up by its soname, so expose both under the names an installed copy would have
HEADERS	= $(ROOT)/build/test-include
LIBDIR	= $(ROOT)/build/test-lib

all: test

compare_nonobject.so: compare_nonobject.cpp $(LIBRARY)
	mkdir -p $(HEADERS) $(LIBDIR)
	ln -sfn $(ROOT)/include $(HEADERS)/phpcpp
	ln -sf $(LIBRARY) $(LIBDIR)/libphpcpp.so
	ln -sf $(LIBRARY) $(LIBDIR)/$(SONAME)
	g++ -Wall -O2 -std=c++11 -fpic -shared -I$(ROOT) -I$(HEADERS) -o $@ $< -L$(LIBDIR) -lphpcpp

test: compare_nonobject.so
	LD_LIBRARY_PATH=$(LIBDIR) php -d extension=$(CURDIR)/compare_nonobject.so test.php

clean:
	rm -f compare_nonobject.so
