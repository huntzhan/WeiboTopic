add_executable(test_item test_item.cc)
target_link_libraries(test_item item_related)

add_executable(test_itemset test_itemset.cc)
target_link_libraries(test_itemset item_related)
target_link_libraries(test_itemset dm_utils)

add_executable(test_cluster test_cluster.cc)
target_link_libraries(test_cluster cluster_related)

add_executable(test_math test_math.cc)
target_link_libraries(test_math dm_utils)

add_gtest(test_item)
add_gtest(test_itemset)
add_gtest(test_cluster)
add_gtest(test_math)
