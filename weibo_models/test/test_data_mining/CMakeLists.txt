# required dataset.

add_executable(test_item test_item.cc)
target_link_libraries(test_item item_related)
add_gtest(test_item)
