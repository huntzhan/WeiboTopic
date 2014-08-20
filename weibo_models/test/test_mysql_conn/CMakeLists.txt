add_executable(test_mysql test_mysql.cc)
target_link_libraries(test_mysql mysql_handler)
add_gtest(test_mysql)
