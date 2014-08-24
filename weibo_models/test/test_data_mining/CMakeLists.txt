add_executable(test_item test_item.cc)
target_link_libraries(test_item item_related)

add_executable(test_itemset test_itemset.cc)
target_link_libraries(test_itemset item_related)

add_executable(test_cluster test_cluster.cc)
target_link_libraries(test_cluster cluster_related)

add_gtest(test_item)
add_gtest(test_itemset)
add_gtest(test_cluster)
