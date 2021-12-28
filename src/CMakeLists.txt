add_library(ring_buffer ring_buffer.cc ring_buffer.hpp)
target_include_directories(ring_buffer PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}")
