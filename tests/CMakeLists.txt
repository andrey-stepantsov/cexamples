
include(FetchContent)
FetchContent_Declare(
	googletest
	URL https://github.com/google/googletest/archive/refs/tags/release-1.11.0.zip
)
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)

FetchContent_MakeAvailable(googletest)

add_executable(
  hello_test
  hello_test.cc
)
target_link_libraries(
  hello_test
  gtest_main
)
add_executable(
  ring_buffer_test
  ring_buffer_test.cc
)
add_executable(
  ring_buffer_thread_test
  ring_buffer_thread_test.cc
)
target_link_libraries(
  ring_buffer_test
  ring_buffer
  gtest_main
)
target_link_libraries(
  ring_buffer_thread_test
  ring_buffer
  gtest_main
)

include(GoogleTest)
gtest_discover_tests(hello_test)
gtest_discover_tests(ring_buffer_test)
gtest_discover_tests(ring_buffer_thread_test)
