
set(DIMENSION_REDUCER dimension_reducer.h)

add_library(dimension_reducer ${DIMENSION_REDUCER})
set_target_properties(dimension_reducer
                      PROPERTIES
                      LINKER_LANGUAGE CXX)
