
include_directories(${weibo_analyse_SOURCE_DIR}/lib)
set(TEXT_SPLIT Textspilt.h Textspilt.cpp)
set(TEXT_SPLIT Textspilt.h Textspilt.cpp)
set(PARSER parser.h parser.cpp)
add_library(parser ${TEXT_SPLIT} ${PARSER})
target_link_libraries(parser
${weibo_analyse_SOURCE_DIR}/lib/ICTCLAS50/libICTCLAS50.so)
target_link_libraries(parser /usr/local/lib/libboost_regex.a)
