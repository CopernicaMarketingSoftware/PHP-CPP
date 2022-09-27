function(find_php_dev)

   find_program(PHP_CONFIG NAMES php-config)

   if (PHP_CONFIG)

      # get include dirs
      execute_process(COMMAND php-config --includes
         OUTPUT_VARIABLE php_CONFIG_INCS
         RESULT_VARIABLE RET
         ERROR_QUIET )

      if(RET EQUAL 0)
         string(STRIP "${php_CONFIG_INCS}" php_CONFIG_INCS)
         separate_arguments(php_CONFIG_INCS_LIST NATIVE_COMMAND "${php_CONFIG_INCS}")
         # parse include dirs, drop -I prefixes
         set(php_INCLUDE_DIRS)
         foreach(arg IN LISTS php_CONFIG_INCS_LIST)
            if("${arg}" MATCHES "^-I(.*)$")
               list(APPEND php_INCLUDE_DIRS "${CMAKE_MATCH_1}")
            endif()
         endforeach()
      endif()

      # get libs
      execute_process(COMMAND php-config --libs
         OUTPUT_VARIABLE php_CONFIG_LIBS
         RESULT_VARIABLE RET
         ERROR_QUIET )

      if(RET EQUAL 0)
         string(STRIP "${php_CONFIG_LIBS}" php_CONFIG_LIBS)
         separate_arguments(php_CONFIG_LIBS_LIST NATIVE_COMMAND "${php_CONFIG_LIBS}")
         # parse libraries, drop -l prefixes
         set(php_DEPENDS_LIBS)
         foreach(arg IN LISTS php_CONFIG_LIBS_LIST)
            if("${arg}" MATCHES "^-l(.*)$")
               list(APPEND php_DEPENDS_LIBS "${CMAKE_MATCH_1}")
            endif()
         endforeach()
      endif()

      # get ldflags
      execute_process(COMMAND php-config --ldflags
         OUTPUT_VARIABLE php_CONFIG_LDFLAGS
         RESULT_VARIABLE RET
         ERROR_QUIET )

      if(RET EQUAL 0)
         string(STRIP "${php_CONFIG_LDFLAGS}" php_LDFLAGS)
      endif()

      set(php_DEV_FOUND    TRUE PARENT_SCOPE)
      set(php_INCLUDE_DIRS ${php_INCLUDE_DIRS} PARENT_SCOPE)
      set(php_DEPENDS_LIBS ${php_DEPENDS_LIBS} PARENT_SCOPE)
      set(php_LDFLAGS      ${php_LDFLAGS}      PARENT_SCOPE)

   else()

      set(php_DEV_FOUND     FALSE PARENT_SCOPE)
      set(php_INCLUDE_DIRS  PARENT_SCOPE)
      set(php_DEPENDS_LIBS  PARENT_SCOPE)
      set(php_LDFLAGS       PARENT_SCOPE)

   endif(PHP_CONFIG)

endfunction()