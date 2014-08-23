
set(ITEM_RELATED item_related.h item_related.cc)
add_library(item_related ${ITEM_RELATED})
target_link_libraries(item_related dimension_reducer)
