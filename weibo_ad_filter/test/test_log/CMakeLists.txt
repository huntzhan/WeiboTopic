
add_executable(test_log test_log.cpp)
target_link_libraries(test_log log)
add_gtest(test_log)
