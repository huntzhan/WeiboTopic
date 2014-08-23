
set(ITEM_RELATED item_related.h item_related.cc)
set(CLUSTER_RELATED cluster_related.h cluster_related.cc)

add_library(item_related ${ITEM_RELATED})
target_link_libraries(item_related dimension_reducer)

add_library(cluster_related ${CLUSTER_RELATED})
