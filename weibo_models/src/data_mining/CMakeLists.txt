
set(ITEM_RELATED item_related.h item_related.cc)
set(CLUSTER_RELATED cluster_related.h cluster_related.cc)
set(DM_UTILS utils.h utils.cc)

add_library(dm_utils ${DM_UTILS})

add_library(item_related ${ITEM_RELATED})
target_link_libraries(item_related dimension_reducer)
target_link_libraries(item_related dm_utils)

add_library(cluster_related ${CLUSTER_RELATED})
target_link_libraries(cluster_related item_related)
target_link_libraries(cluster_related calculator)
target_link_libraries(cluster_related dm_utils)
