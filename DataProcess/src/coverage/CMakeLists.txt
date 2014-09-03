
set(coverage_src coverage.h coverage.cpp)
set(parser_src parser.h parser.cpp)
set(simhash_src simhash.h simhash.cpp)
set(textsplit_src Textspilt.h Textspilt.cpp)

add_library(coverage_lib ${coverage_src})
add_library(parser_lib ${parser_src})
add_library(simhash_lib ${simhash_src})
add_library(textsplit_lib ${textsplit_src})


target_link_libraries(textsplit_lib -lboost_regex ${dataprocess_SOURCE_DIR}/lib/ICTCLAS50/libICTCLAS50.a)
target_link_libraries(parser_lib textsplit_lib)
target_link_libraries(simhash_lib parser_lib)
target_link_libraries(coverage_lib dboperation)
