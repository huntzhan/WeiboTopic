
set(LOGGER_SRC logger.h logger.cpp) 
add_library(logger ${LOGGER_SRC})
target_link_libraries(logger model db_operation)

set(LOG_SRC log.cpp) 
add_library(log ${LOG_SRC})
