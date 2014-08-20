
include_directories(${weibo_ad_filter_SOURCE_DIR}/lib)
add_subdirectory(DB)  # utility should be built before everything else.
add_subdirectory(split)  # utility should be built before everything else.

add_executable(run process.cpp preprocessor.cpp tactic.cpp)
target_link_libraries(run db_pool)
target_link_libraries(run connection_pool)
target_link_libraries(run parser)
target_link_libraries(run db_operation)
target_link_libraries(run model)
target_link_libraries(run /usr/local/lib/libboost_regex.a)
target_link_libraries(run ${weibo_ad_filter_SOURCE_DIR}/lib/ICTCLAS50/libICTCLAS50.so)
