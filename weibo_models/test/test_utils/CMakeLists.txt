# required dataset.
file(INSTALL ${CPPJIEBA}/dict/jieba.dict.utf8
             ${CPPJIEBA}/dict/hmm_model.utf8
             ${CPPJIEBA}/dict/idf.utf8
             ${CPPJIEBA}/dict/stop_words.utf8
     DESTINATION data/jieba)

add_executable(test_dimension_reducer test_dimension_reducer.cc)
target_link_libraries(test_dimension_reducer dimension_reducer)
target_link_libraries(test_dimension_reducer mysql_handler)

add_executable(test_text_cleaner test_text_cleaner.cc)
target_link_libraries(test_text_cleaner text_cleaner)

add_gtest(test_dimension_reducer)
add_gtest(test_text_cleaner)
