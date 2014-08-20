
set(TACTIC_SRC tactic.cpp) 
add_library(tactic ${TACTIC_SRC})
target_link_libraries(tactic model)
target_link_libraries(tactic parser)


set(PREPROCESSOR_SRC preprocessor.h preprocessor.cpp)
add_library(preprocessor ${PREPROCESSOR_SRC}) 
target_link_libraries(preprocessor model)
