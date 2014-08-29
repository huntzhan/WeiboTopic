
set(ALLO_SRC allocator.cpp) 
add_library(allocator ${ALLO_SRC})
target_link_libraries(allocator model db_operation log)

