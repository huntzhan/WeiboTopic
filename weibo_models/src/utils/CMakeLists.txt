set(DIMENSION_REDUCER dimension_reducer.h)
set(TEXT_CLEANER text_cleaner.h text_cleaner.cc)
set(CALCULATOR calculator.h)

add_library(text_cleaner ${TEXT_CLEANER})
find_library(BOOST_REGEX
             NAMES boost_regex
             PATHS /usr/lib)
target_link_libraries(text_cleaner ${BOOST_REGEX})

add_library(dimension_reducer ${DIMENSION_REDUCER})
set_target_properties(dimension_reducer
                      PROPERTIES
                      LINKER_LANGUAGE CXX)
target_link_libraries(dimension_reducer text_cleaner)

add_library(calculator ${CALCULATOR})
set_target_properties(calculator
                      PROPERTIES
                      LINKER_LANGUAGE CXX)
