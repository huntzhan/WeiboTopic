
set(TEXT_SPLIT Textspilt.cpp)
add_library(textsplit ${TEXT_SPLIT})
target_link_libraries(textsplit ${weibo_ad_filter_SOURCE_DIR}/lib/ICTCLAS50/libICTCLAS50.so)
# target_link_libraries(textsplit ${LIB_ROOT}/ICTCLAS50/libICTCLAS50.so)
target_link_libraries(textsplit -lboost_regex)

set(PARSER parser.cpp)
add_library(parser ${PARSER})
target_link_libraries(parser textsplit)
