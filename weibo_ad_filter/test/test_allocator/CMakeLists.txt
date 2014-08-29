
add_executable(test_allocator test_allocator.cpp)
target_link_libraries(test_allocator allocator)
add_gtest(test_allocator)
