
# set(KEYWORD_SRC keyword.h) 
# add_library(keyword ${KEYWORD_SRC})

set(BAYES_SRC bayes_classifier.h) 
add_library(bayes ${BAYES_SRC})
target_link_libraries(bayes parsedblog log)
