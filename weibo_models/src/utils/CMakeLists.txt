
set(DIMENSION_REDUCER dimension_reducer.h)
set(TEXT_CLEANER text_cleaner.h text_cleaner.cc)

add_library(dimension_reducer ${DIMENSION_REDUCER})
set_target_properties(dimension_reducer
                      PROPERTIES
                      LINKER_LANGUAGE CXX)

add_library(text_cleaner ${TEXT_CLEANER})
find_library(BOOST_REGEX
             NAMES boost_regex
             PATHS /usr/lib)
target_link_libraries(text_cleaner ${BOOST_REGEX})
