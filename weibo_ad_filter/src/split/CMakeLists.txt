
set(TEXT_SPLIT Textspilt.cpp)
add_library(textsplit ${TEXT_SPLIT})
target_link_libraries(textsplit ${weibo_ad_filter_SOURCE_DIR}/lib/ICTCLAS50/libICTCLAS50.so)
# target_link_libraries(textsplit ${LIB_ROOT}/ICTCLAS50/libICTCLAS50.so)
<<<<<<< HEAD
target_link_libraries(textsplit /usr/lib/libboost_regex.a)
=======
target_link_libraries(textsplit -lboost_regex)
>>>>>>> 36eaa3f92257536cf9394c65aa780020aba2bf82

set(PARSER parser.cpp)
add_library(parser ${PARSER})
target_link_libraries(parser textsplit)
