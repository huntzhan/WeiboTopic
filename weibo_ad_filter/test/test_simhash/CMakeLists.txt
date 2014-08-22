
add_executable(test_simhash test_simhash.cpp)
target_link_libraries(test_simhash simhash) 
add_gtest(test_simhash)
