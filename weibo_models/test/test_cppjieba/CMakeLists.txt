add_executable(jieba_demo demo.cc)
set_target_properties(jieba_demo
                      PROPERTIES
                      COMPILE_FLAGS -DLOGGER_LEVEL=LL_WARN)
                      
file(INSTALL ${CPPJIEBA}/dict/jieba.dict.utf8
             ${CPPJIEBA}/dict/hmm_model.utf8
             ${CPPJIEBA}/dict/idf.utf8
             ${CPPJIEBA}/dict/stop_words.utf8
     DESTINATION data/jieba)
