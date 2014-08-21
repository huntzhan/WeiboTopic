
set(SIMHASH_SRC simhash.cpp) 
add_library(simhash ${SIMHASH_SRC})
target_link_libraries(simhash parser)



