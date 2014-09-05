set(connection_pool_src connection_pool.h connection_pool.cpp)
set(dboperation_src DBoperation.h  DBoperation.cpp)
set(timetool_src TimeTool.h)
add_library(connection_pool ${connection_pool_src})
add_library(dboperation ${dboperation_src})
target_link_libraries(connection_pool -lmysqlcppconn libboost_regex.a)
target_link_libraries(dboperation -lmysqlclient   libboost_regex.a)

