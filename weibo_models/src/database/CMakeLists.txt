find_library(MYSQLCPPCONNN
             NAMES mysqlcppconn
             PATHS ${MYSQL_CONNECT_PATH}/lib)

set(MYSQL_HANDLER mysql_handler.h mysql_handler.cc)
add_library(mysql_handler ${MYSQL_HANDLER})
target_link_libraries(mysql_handler ${MYSQLCPPCONNN})
