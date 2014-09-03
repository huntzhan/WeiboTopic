#genrate the bayes lib
SET(TRAIN_SRC TrainModel.h TrainModel.cpp)
SET(TEST_SRC TestModel.h TestModel.cpp)
add_library(bayes_train ${TRAIN_SRC})
add_library(bayes ${TEST_SRC})
target_link_libraries(bayes bayes_train)
target_link_libraries(bayes ${dataprocess_SOURCE_DIR}/lib/ICTCLAS50/libICTCLAS50.a)
target_link_libraries(bayes ${dataprocess_SOURCE_DIR}/lib/REGEX/libboost_regex.a)
