
set(REF_SRC ref.h) 
add_library(ref ${REF_SRC})
target_link_libraries(ref simhash)
