
include_directories(${weibo_ad_filter_SOURCE_DIR}/src)
add_executable(test_tactic test_tactic.cpp)
target_link_libraries(test_tactic db_operation model logger tactic parser) 
add_gtest(test_tactic)
