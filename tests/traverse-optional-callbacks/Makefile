# Builds the test extension against the library in the repository root (run 'make' there first) and runs the test script.
ROOT	= $(abspath ../..)
LIBRARY	= $(wildcard $(ROOT)/libphpcpp.so.*.*.*)
SONAME	= $(shell readelf -d $(LIBRARY) | sed -n 's/.*soname: \[\(.*\)\]/\1/p')

# scratch directories inside the git-ignored build/ directory: the headers include each other as <phpcpp/...> and the
# library is looked up by its soname, so expose both under the names an installed copy would have
HEADERS	= $(ROOT)/build/test-include
LIBDIR	= $(ROOT)/build/test-lib

all: test

traverse_pairs.so: traverse_pairs.cpp $(LIBRARY)
	mkdir -p $(HEADERS) $(LIBDIR)
	ln -sfn $(ROOT)/include $(HEADERS)/phpcpp
	ln -sf $(LIBRARY) $(LIBDIR)/libphpcpp.so
	ln -sf $(LIBRARY) $(LIBDIR)/$(SONAME)
	g++ -Wall -O2 -std=c++11 -fpic -shared -I$(ROOT) -I$(HEADERS) -o $@ $< -L$(LIBDIR) -lphpcpp

test: traverse_pairs.so
	LD_LIBRARY_PATH=$(LIBDIR) php -d extension=$(CURDIR)/traverse_pairs.so test.php

clean:
	rm -f traverse_pairs.so
