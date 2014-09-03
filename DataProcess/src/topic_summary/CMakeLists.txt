set(topicview_src TopicView.h TopicView.cpp)
set(topicview_and_politics TopicViewAndPolitics.h TopicViewAndPolitics.cpp)
add_library(topicviewpolictics ${topicview_src} ${topicview_and_politics})
target_link_libraries(topicviewpolictics ispolitics test)

