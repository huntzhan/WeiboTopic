include_directories(${weibo_ad_filter_SOURCE_DIR}/src)
add_executable(test_db test_db.cpp)
target_link_libraries(test_db connection_pool db_pool db_operation) 
add_gtest(test_db)
