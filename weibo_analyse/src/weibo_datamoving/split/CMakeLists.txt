
set(TEXT_SPLIT Textspilt.h Textspilt.cpp)
add_library(text_split ${TEXT_SPLIT})
target_link_libraries(text_split
${weibo_analyse_SOURCE_DIR}/lib/ICTCLAS50/libICTCLAS50.so)
target_link_libraries(text_split
${weibo_analyse_SOURCE_DIR}/lib/boost/libboost_regex.a )
