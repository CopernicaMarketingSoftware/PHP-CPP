# usage: make PHPCPP_LIB=/path/to/dir/with/libphpcpp.so   (defaults to the system install)
PHPCPP_LIB ?= /usr/lib
EXTENSION_DIR := $(shell php-config --extension-dir)

traverse_leak.so: traverse_leak.cpp
	g++ -Wall -O2 -std=c++11 -fpic -shared -o $@ $< -L$(PHPCPP_LIB) -lphpcpp

clean:
	rm -f traverse_leak.so
