
include(FetchContent)
FetchContent_Declare(
	googletest
	URL https://github.com/google/googletest/archive/refs/tags/release-1.11.0.zip
)
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)

FetchContent_MakeAvailable(googletest)

add_executable( ring_buffer_test ring_buffer_test.cc)
target_link_libraries( ring_buffer_test ring_buffer gtest_main)

add_executable( ring_buffer_thread_test ring_buffer_thread_test.cc)
target_link_libraries( ring_buffer_thread_test ring_buffer time_util gtest_main)

add_executable( explore_time_test explore_time_test.cc)
target_link_libraries( explore_time_test time_util gtest_main)

add_executable( post_order_traversal_test post_order_traversal_test.cc)
target_link_libraries( post_order_traversal_test gtest_main )

include(GoogleTest)
gtest_discover_tests(ring_buffer_test)
gtest_discover_tests(ring_buffer_thread_test)
gtest_discover_tests(explore_time_test)
gtest_discover_tests(post_order_traversal_test)
