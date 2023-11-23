if (NOT SAMPLE_NAME)
   message(FATAL_ERROR "SAMPLE_NAME is not defined!")
endif ()

if(NOT SAMPLE_SOURCES)
   set(SAMPLE_SOURCES ${SAMPLE_NAME}.cpp)
endif()

if(NOT SAMPLE_PHPFILE)
   set(SAMPLE_PHPFILE ${SAMPLE_NAME}.php)
endif()

SET (SAMPLE_PHPFILE_DEST ${SAMPLE_NAME}.php)

# -----------------------------------------------------------------------------


add_library(${SAMPLE_NAME} MODULE )
target_sources(${SAMPLE_NAME} PRIVATE ${SAMPLE_SOURCES} )
target_include_directories(${SAMPLE_NAME} PRIVATE ${PHPCPP_INCLUDE_DIR})
target_link_libraries(${SAMPLE_NAME} PRIVATE phpcpp)

if(WIN32)
   if (PHPCPP_BUILD_SHARED)
      target_compile_definitions(${SAMPLE_NAME} PRIVATE USING_PHPCPP_DLL )
   endif()
else()
   target_link_options(${SAMPLE_NAME} PRIVATE -shared )
   target_compile_options(${SAMPLE_NAME} PRIVATE -c -Wall )
endif()



# Copy binaries, resources to _output folder
# -----------------------------------------------------------------------------
set(OUTPUT_EXTDIR "${PHPCPP_OUTPUT_DIR}/ext_static")
if (PHPCPP_BUILD_SHARED)
  set(OUTPUT_EXTDIR "${PHPCPP_OUTPUT_DIR}/ext_shared")
endif()

# Copy extension to _output dir
add_custom_command(TARGET ${SAMPLE_NAME} POST_BUILD
   COMMAND ${CMAKE_COMMAND} -E make_directory ${OUTPUT_EXTDIR}/$<$<CONFIG:Debug>:debug>$<$<CONFIG:Release>:release>
   COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_FILE:${SAMPLE_NAME}> ${OUTPUT_EXTDIR}/$<$<CONFIG:Debug>:debug>$<$<CONFIG:Release>:release> )

# Copy php sample to _output dir
if(EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/${SAMPLE_PHPFILE})
   add_custom_command(TARGET ${SAMPLE_NAME} POST_BUILD
      COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_CURRENT_SOURCE_DIR}/${SAMPLE_PHPFILE} ${OUTPUT_EXTDIR}/$<$<CONFIG:Debug>:debug>$<$<CONFIG:Release>:release>/${SAMPLE_PHPFILE_DEST} )
endif()