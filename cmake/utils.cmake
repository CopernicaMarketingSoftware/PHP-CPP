function(find_php_dev phpcfg)

   set(PHP_CONFIG_CMD ${phpcfg})

   find_program(PHP_CONFIG NAMES ${PHP_CONFIG_CMD})

   if (PHP_CONFIG)
      # get include dirs
      execute_process(COMMAND ${PHP_CONFIG_CMD} --includes
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
      execute_process(COMMAND ${PHP_CONFIG_CMD} --libs
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
      execute_process(COMMAND ${PHP_CONFIG_CMD} --ldflags
         OUTPUT_VARIABLE php_CONFIG_LDFLAGS
         RESULT_VARIABLE RET
         ERROR_QUIET )

      if(RET EQUAL 0)
         string(STRIP "${php_CONFIG_LDFLAGS}" php_LDFLAGS)
      endif()

      # get version number
      execute_process(COMMAND ${PHP_CONFIG} --vernum
         OUTPUT_VARIABLE php_CONFIG_VERNUM
         RESULT_VARIABLE RET
         ERROR_QUIET )

      if(RET EQUAL 0)
         string(STRIP "${php_CONFIG_VERNUM}" php_VERNUMBER)
      endif()


      set(PHP_DEV_FOUND    TRUE PARENT_SCOPE)
      set(PHP_INCLUDE_DIRS ${php_INCLUDE_DIRS} PARENT_SCOPE)
      set(PHP_DEPENDS_LIBS ${php_DEPENDS_LIBS} PARENT_SCOPE)
      set(PHP_LDFLAGS      ${php_LDFLAGS}      PARENT_SCOPE)
      set(PHP_VERNUMBER    ${php_VERNUMBER}    PARENT_SCOPE)

   else()

      set(PHP_DEV_FOUND      FALSE PARENT_SCOPE)
      set(PHP_INCLUDE_DIRS   "" PARENT_SCOPE)
      set(PHP_DEPENDS_LIBS   "" PARENT_SCOPE)
      set(PHP_LDFLAGS        "" PARENT_SCOPE)
      set(PHP_VERSION_NUMBER "" PARENT_SCOPE)

   endif(PHP_CONFIG)

endfunction()