This is just a proof of concept C++ PHP module that loads and runs WASM using https://github.com/extism/extism 

as an alternative to https://github.com/extism/php-sdk , which has the following issues: 

1 - uses C bindings, not C++

2 - uses php's FFI interface, which itself is a php loadable module like this one, so its got more levels of indirection

3 - PHP FFI is slower that C++ bindings.


This is just barely enough to proove it worked for me, and no more.

Here some output: 
```
cd ~/PHP-CPP/Examples/extism
[buzz@qbi-buzz extism]$ make clean ; make ; sudo make install ; time php extismcpp.php
rm -f extismcpp.so extismcpp.o
sudo rm -f /usr/lib/php/20220829/extismcpp.so 
sudo rm -f /etc/php/8.2/cli/conf.d//extismcpp.ini 
g++ -Wall -c -O2 -std=c++11 -fpic -o extismcpp.o extismcpp.cpp
g++ -shared -o extismcpp.so extismcpp.o -lphpcpp -lextism
cp -f extismcpp.so /usr/lib/php/20220829
cp -f extismcpp.ini /etc/php/8.2/cli/conf.d/
ExtismClass::ExtismClass([no params])
C++ reading wasm...
reading WASM with read_file
done WASM with read_file length:18258
C++ reading wasm done.
calling extism_function_new with hello_world
calling extism_plugin_new with hello function
sending a string to count to count_vowels...
Hello from hello_world C++!
0x7f6583a7135b
hello_world:{"count": 6}[hello output done]
print_plugin_output:{"count": 6}[output done]
count_vowels plugin said:{"count": 6}12
Freeing userdata
------------------------------
LoadWASM count vowels result: {"count": 6}
------------------------------
ExtismClass::__destruct
ExtismClass::~ExtismClass

real	0m0.037s
user	0m0.029s
sys	0m0.008s
```
Yup, so with this... PHP can instantiate extism using its C++ bindings, load a wasm, register a plugin and call a function.   I leave it to u to make the interface sane, and not hardcoding a bunch of stuff.

