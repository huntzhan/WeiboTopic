
include_directories(${weibo_ad_filter_SOURCE_DIR}/lib)
include_directories(${weibo_ad_filter_SOURCE_DIR}/src)
add_executable(test_parser test_parser.cpp)
target_link_libraries(test_parser parser) 
add_gtest(test_parser)
