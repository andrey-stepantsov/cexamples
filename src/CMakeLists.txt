add_library(ring_buffer ring_buffer.cc ring_buffer.hpp)
target_include_directories(ring_buffer PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}")

add_library(time_util time_util.cc time_util.hpp)
target_include_directories(time_util PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}")

