
set(TACTIC_SRC tactic.cpp) 
add_library(tactic ${TACTIC_SRC})
target_link_libraries(tactic model)
target_link_libraries(tactic parser)
target_link_libraries(tactic log)

set(PREPROCESSOR_SRC preprocessor.h preprocessor.cpp)
add_library(preprocessor ${PREPROCESSOR_SRC}) 
target_link_libraries(preprocessor model)
target_link_libraries(preprocessor simhash)
target_link_libraries(preprocessor mysql_handler)
target_link_libraries(tactic log)
