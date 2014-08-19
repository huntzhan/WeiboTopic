find_library(MYSQLCPPCONNN
             NAMES mysqlcppconn
             PATHS ${MYSQL_CONNECT_PATH}/lib)

add_executable(test_mysql test_mysql.cc)
target_link_libraries(test_mysql ${MYSQLCPPCONNN})
add_gtest(test_mysql)
