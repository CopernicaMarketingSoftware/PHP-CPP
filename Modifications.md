

## Modify CMake definitions
   - CMakeLists.txt
      * Minimum version 3.11
   - Add cmake/sources.cmake, cmake/utils.cmake, cmake/sample.cmake
   - Move zend/streambuf.cpp to phpcpp-common sources list, because  of runtime error:  undefined symbol: _ZN3Php9StreamBuf4syncEv
      in static library build

## Add file zend/modifiers.cpp
   - Add definitions for the enums, because of link error in shared library build - undefined


## Modify include/visibility.h
   - Add macro MODULE_EXPORT, for building extension
   - Add macro USING_PHPCPP_DLL, for building extension with php-cpp built as shared lib(dll)


## Modify include/array.h
   - Modify constructor
      * Array(const std::initializer_list<Value> &value)
   Note:
      - Shared and Static library compiled,
      - Link error occured when compiling sample - FunctionWithParameter
      - link error LNK2019: unresolved external symbol "public: __cdecl Php::Value::Value<class Php::Value>(class std::initializer_list<class Php::Value> const &)"


## Modify include/call.h
   - Remove PHPCPP_EXPORT from function declarations
      - extern PHPCPP_EXPORT    Value constant(const char *constant, size_t size);
      - extern PHPCPP_EXPORT    bool  define(const char *name, size_t size, const Value &value);
      - extern PHPCPP_EXPORT    Value set_exception_handler(const std::function<Value(Parameters &params)> &handler);
      - extern PHPCPP_EXPORT    Value set_error_handler(const std::function<Value(Parameters &params)> &handler, Message message = Message::All);
   - Note:
      - compile error 'Php::constant': redefinition; different linkage


## Modify include/class.handler
   - Remove PHPCPP_EXPORT from Class template


## Modify include/file.h and zend/file.cpp
   - Move constructor definition to source file
      -  File(const char *name)
      -  File(const std::string &name)
      -  File(const Value &value)
   - Note :
      - Shared and Static library compiled,
      - Link error occured when compiling sample - FunctionWithParameter
      - error LNK2019: unresolved external symbol "const Php::File::`vftable'"


## Modify include/noexcept.h
   - Add test for _MSC_VER >= 1400 , enable noexcept


## Modify include/streams.h
   - Remove PHPCPP_EXPORT from function declarations
   - Note :
      - compile error data with thread storage duration may not have dll interface


## Modify zend/parametersimpl.h
   - Modify contructor
      - ParametersImpl(zval *this_ptr, uint32_t argc)
            - old: zval arguments[argc];
            - new: zval* arguments = new zval[argc];
        - Note: compile error C2131: expression did not evaluate to a constant


## Modify include/value.h
   - Move constructor definition to zend/value.cpp
      - Construct to a specific type
      - Constructor from an initializer list
   - Note :
      - Shared and Static library compiled,
      - Link error occured when compiling sample - FunctionWithParameter
      - error LNK2019: unresolved external symbol "const Php::Value::`vftable'" (??_7Value@Php@@6B@) referenced in function "public: __cdecl Php::Value::Value(enum Php::Type)"


## Modify include/module.h
   - Add cast to HMODULE in virtual ~Persistent()


## Modify zend/callable.cpp
   - Add cast in function definition
   - void Callable::initialize(zend_function_entry *entry, const char *classname, int flags) const
      - add cast: (zif_handler)_callback
      - add cast: (zif_handler)&Callable::invoke


## Modify zend/classimpl.cpp
   - Add cast in function definition
      - zend_function *ClassImpl::getMethod(zend_object **object, zend_string *method, const zval *key)
           - add cast: (zif_handler)&ClassImpl::callMethod
      - zend_function *ClassImpl::getStaticMethod(zend_class_entry *entry, zend_string *method)
           - add cast: (zif_handler)&ClassImpl::callMethod
      - int ClassImpl::getClosure(zval *object, zend_class_entry **entry_ptr, zend_function **func, zend_object **object_ptr)
            - add cast: (zif_handler)&ClassImpl::callInvoke

## Modify zend/value.cpp
   - Modify function definition
      - Value::exec(int argc, Value *argv) const definition:
      - Value Value::exec(const char *name, int argc, Value *argv) const
      - Value Value::exec(const char *name, int argc, Value *argv) const
           - old: zval params[argc];
           - new: zval* params = new zval[argc];
        - Note :
           - compile error C2131: expression did not evaluate to a constant

      - static Value do_exec(const zval *object, zval *method, int argc, zval *argv) definition:
           - delete [] argv;
           
## Sample Extension
   - Replace PHPCPP_EXPORT with MODULE_EXPORt
   - Linking with php-cpp built as dll, require defining USING_PHPCPP_DLL