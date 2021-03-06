# An example C11 project

cmake_minimum_required(VERSION 3.15)

project(CTutor VERSION 1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 11)

enable_testing()
set(CMAKE_EXPORT_COMPILE_COMMANDS 1)

message("CMAKE_DEBUG_POSTFIX = ${CMAKE_DEBUG_POSTFIX}")

set(THREADS_PREFER_PTHREAD_FLAG TRUE)
find_package( Threads REQUIRED )
if (Threads_FOUND) 
  message("Threads_FOUND = ${Threads_FOUND}")
  message("CMAKE_USE_PTHREADS_INIT = ${CMAKE_USE_PTHREADS_INIT}")
endif(Threads_FOUND)

add_subdirectory(src)
add_subdirectory(tests)
# add_subdirectory(apps)

option(BUILD_DOC "Build documentation" ON)
find_package(Doxygen)

if (DOXYGEN_FOUND)
    # set input and output files
    set(DOXYGEN_IN ${CMAKE_CURRENT_SOURCE_DIR}/docs/Doxyfile.in)
    set(DOXYGEN_OUT ${CMAKE_CURRENT_BINARY_DIR}/Doxyfile)

    # request to configure the file
    configure_file(${DOXYGEN_IN} ${DOXYGEN_OUT} @ONLY)
    message("Doxygen build started")

    # note the option ALL which allows to build the docs together with the application
    add_custom_target( doc_doxygen ALL
        COMMAND ${DOXYGEN_EXECUTABLE} ${DOXYGEN_OUT}
        WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
        COMMENT "Generating API documentation with Doxygen"
        VERBATIM )
else (DOXYGEN_FOUND)
  message("Doxygen need to be installed to generate the doxygen documentation")
endif (DOXYGEN_FOUND)
