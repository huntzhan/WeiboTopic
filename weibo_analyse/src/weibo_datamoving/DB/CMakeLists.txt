
set(CONNECTION_POOL connection_pool.h connection_pool.cpp)
set(DB_POOL DBpool.h DBpool.cpp) 
set(DB_OPERATION DBoperation.h DBoperation.cpp) 
add_library(connection_pool ${CONNECTION_POOL})
add_library(db_pool ${DB_POOL})
add_library(db_operation ${DB_OPERATION})
