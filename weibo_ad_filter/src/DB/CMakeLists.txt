
include_directories(${weibo_ad_filter_SOURCE_DIR}/lib)
set(CONNECTION_POOL connection_pool.h connection_pool.cpp)
set(DB_POOL DBpool.h DBpool.cpp) 
set(DB_OPERATION DBoperation.h DBoperation.cpp) 
add_library(connection_pool ${CONNECTION_POOL})
add_library(db_pool ${DB_POOL})
add_library(db_operation ${DB_OPERATION})
target_link_libraries(connection_pool -lmysqlclient -lmysqlcppconn -lpthread /usr/local/lib/libboost_regex.a) 
target_link_libraries(db_operation -lmysqlclient -lmysqlcppconn -lpthread /usr/local/lib/libboost_regex.a) 
target_link_libraries(db_pool -lmysqlclient -lmysqlcppconn -lpthread /usr/local/lib/libboost_regex.a) 
# target_link_libraries(CONNECTION_POOL 
# target_link_libraries(DB_OPERATION -llibboost_regex.a)
# target_link_libraries(DB_POOL -llibboost_regex.a)
