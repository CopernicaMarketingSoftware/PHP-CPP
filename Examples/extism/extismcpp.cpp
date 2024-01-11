/**
 *  extismcpp.cpp
 *  @author Jasper van Eck<jasper.vaneck@copernica.com>
 * 
 *  An example file to show the working of using a C++ class in PHP.
 */

#include <string>
#include <iostream>

/**
 * 	Our own library/s
 */
#include <phpcpp.h>  // /usr/include/phpcpp.h
#include <extism.h>  // /usr/local/include/extism.h

#include <errno.h>
#include <cassert>

#include <fstream>

#include <bits/stdc++.h> 

/**
 *  Namespace to use
 */
using namespace std;

Php::Value my_no_parameters_function()
{
    return "42";
}

/**
 *  This functions receives a reference to a variable. When the variable is altered,
 *  so is the value in the php script.
 *  my_with_defined_parameters_reference_function()
 *  @param  Php::Parameters     the given parameters
 */
Php::Value my_with_defined_parameters_reference_function(Php::Parameters &params)
{
    // params[0] = "I changed!";
    // return params[0];

    return "stuff";
}

// a few functions used by extism that are very lightly ported to C++ from C, just enough to compile.

uint8_t *read_file(const char *filename, size_t *len) {

    std::cout << "reading WASM with read_file" << std::endl;

  FILE *fp = fopen(filename, "rb");
  if (fp == NULL) {
    return NULL;
  }
  fseek(fp, 0, SEEK_END);
  size_t length = ftell(fp);
  fseek(fp, 0, SEEK_SET);

  uint8_t *data = (uint8_t*)malloc(length);
  if (data == NULL) {
    fclose(fp);
    return NULL;
  }

  assert(fread(data, 1, length, fp) == length);
  fclose(fp);

  *len = length;

  std::cout << "done WASM with read_file length:" << length  << std::endl;

  return data;
}

void hello_world(ExtismCurrentPlugin *plugin, const ExtismVal *inputs,
                 uint64_t n_inputs, ExtismVal *outputs, uint64_t n_outputs,
                 void *data) {
  std::cout << "Hello from hello_world C++!" << std::endl;

  std::cout << data << std::endl;

  ExtismSize ptr_offs = inputs[0].v.i64;

  uint8_t *buf = extism_current_plugin_memory(plugin) + ptr_offs;
  //uint64_t length = extism_current_plugin_memory_length(plugin, ptr_offs);
  //fwrite(buf, length, 1, stdout);
  //fputc('\n', stdout);

  std::cout << "hello_world:" << buf << "[hello output done]" << std::endl;

  outputs[0].v.i64 = inputs[0].v.i64;
}

void log_handler(const char *line, uintptr_t length) {
  fwrite(line, length, 1, stderr);
}

void print_plugin_output(ExtismPlugin *plugin, int32_t rc){
  if (rc != EXTISM_SUCCESS) {
    //fprintf(stderr, "ERROR: %s\n", extism_plugin_error(plugin));
    std::cout << "ERR" << extism_plugin_error(plugin) << std::endl;
    return;
  }

  //size_t outlen = extism_plugin_output_length(plugin);
  const uint8_t *out = extism_plugin_output_data(plugin);
  //write(STDOUT_FILENO, out, outlen);
  std::cout << "print_plugin_output:" << out << "[output done]" << std::endl;
}

void free_data(void *x) { puts("Freeing userdata"); }


class ExtismClass : public Php::Base // , public Php::Countable
{
private:
    int _x = -1;
    
public:
    ExtismClass()
    {
        std::cout << "ExtismClass::ExtismClass([no params])" << std::endl;
    }

    ExtismClass(int value) : _x(value)
    {
        std::cout << "ExtismClass::ExtismClass(" << value << ")" << std::endl;
    }

    ExtismClass(const ExtismClass &that)
    {
        std::cout << "ExtismClass::ExtismClass copy constructor" << std::endl;
    }
    
    virtual ~ExtismClass()
    {
        std::cout << "ExtismClass::~ExtismClass" << std::endl;
    }

    virtual void __construct()
    {
        std::cout << "ExtismClass::__construct" << std::endl;
    }

    virtual void __destruct()
    {
        std::cout << "ExtismClass::__destruct" << std::endl;
    }
    
    virtual Php::Value count() //override
    {
        return _x;
    }

    Php::Value handlestring(Php::Parameters &params) //override
    {
        if (params.size() != 1) throw Php::Exception("Invalid number of parameters supplied");

        string str = params[0].rawValue(); // the first param is a string. .rawValue() forces the Php::Value to give us a 'const char *'

        std::cout << "C++ handling string:" << str << std::endl;
        return str;
    }

    Php::Value LoadWASM(Php::Parameters &params)
    {
        if (params.size() != 1) throw Php::Exception("Invalid number of parameters supplied");

        // params[0] = the wasm filename

        // the result string is  returned

        string filename = params[0].rawValue(); // the first param is a string. .rawValue() forces the Php::Value to give us a 'const char *'

        // todo use c++ isms to load it here rather than like this... this just proves we can load it really.
        std::cout << "C++ reading wasm..."  << std::endl;

         size_t len = 0;
        uint8_t *data = read_file(filename.c_str(), &len);
        ExtismValType inputs[] = {PTR};
        ExtismValType outputs[] = {PTR};
        char *errmsg = NULL;

        std::cout << "C++ reading wasm done."  << std::endl;

        std::cout << "calling extism_function_new with hello_world" << std::endl;

        ExtismFunction *f =  extism_function_new("hello_world", inputs, 1, outputs, 1, hello_world,  (void*) "Hello, again!", free_data);

        std::cout << "calling extism_plugin_new with hello function" << std::endl;

        ExtismPlugin *plugin = extism_plugin_new(data, len, (const ExtismFunction **)&f, 1, true, &errmsg);
        free(data);
        if (plugin == NULL) {
            puts(errmsg);
            extism_plugin_new_error_free(errmsg);
            exit(1);
        }

        std::cout << "sending a string to count to count_vowels..." << std::endl;
        const char *input = "Hello, count_vowels!";
        print_plugin_output(plugin, extism_plugin_call(plugin, "count_vowels",
                (const uint8_t *)input, strlen(input)));

        ExtismSize out_len = extism_plugin_output_length(plugin);
        const uint8_t *output = extism_plugin_output_data(plugin);
        //write(STDOUT_FILENO, output, out_len);
        std::cout << "count_vowels plugin said:" << output <<  out_len << std::endl;
        //write(STDOUT_FILENO, "\n", 1);

        std::string myString;
        myString.resize(out_len);
        memcpy(&myString[0], &output[0], out_len);

        extism_plugin_free(plugin);
        extism_function_free(f);
        extism_log_drain(log_handler);

        return myString;
    }
    
    Php::Value Factory(Php::Parameters &params)
    {

        // check number of parameters
        if (params.size() != 1) throw Php::Exception("Invalid number of parameters supplied");
        
        std::cout << "Factory called from object " << _x << std::endl;
        std::cout << "Factory returning new object " << params[0] << std::endl;

        return Php::Object("ExtismClass", new ExtismClass(params[0]));

    }
};

// Symbols are exported according to the "C" language
extern "C" 
{
    // export the "get_module" function that will be called by the Zend engine
    PHPCPP_EXPORT void *get_module()
    {
        // create extension
        static Php::Extension extension("extismcpp","1.0");

        // add function to extension
        extension.add<my_no_parameters_function>("my_no_parameters_function");

        // add function, with defined parameter by reference, to extension
        extension.add<my_with_defined_parameters_reference_function>("my_with_defined_parameters_reference_function", {
            Php::ByRef("string", Php::Type::String)
            });
        
        
        // // we are going to define a class
        Php::Class<ExtismClass> customClass("ExtismClass");

        // // add methods to it
        customClass.method<&ExtismClass::Factory>("Factory", Php::Final, {});

        customClass.method<&ExtismClass::LoadWASM>("LoadWASM",Php::Final, {
            Php::ByVal("string", Php::Type::String)//, // filename in
            //Php::ByRef("string", Php::Type::String) // result out by reference
            });

        // customClass.method<&ExtismClass::count>("count");

        // customClass.method<&ExtismClass::handlestring>("handlestring");
        // { 
        //     Php::ByVal("change", Php::Type::String, true);  // true means its a required param, and a string.
        // }

        // customClass.property("property1", "prop1");
        // customClass.property("property2", "prop2");

        // add the class to the extension
        extension.add(customClass);
        
        // return the extension module
        return extension;
    }
}
