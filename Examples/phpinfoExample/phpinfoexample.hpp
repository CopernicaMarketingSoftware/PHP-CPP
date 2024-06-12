#define MODULENAME "PHPInfoExample"
#define MODULENAME_LOWER "phpinfoexample"
#define MODULEVERSION "1.0"
#define MODULE_AUTHOR "Some one"
#define MODULE_COPYRIGHT "Some Company LLC"

#define INFO_FUNC(module) void info__##module(Php::Extension* extension)
#define CALL_INFO_FUNC(module, ex) info__##module(ex);


