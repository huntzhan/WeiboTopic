add_subdirectory(DB)
add_subdirectory(split)

add_executable(a.out process.cpp db_lib lexical_analysis_lib)