cmake_minimum_required(VERSION 2.8)
project(dataprocess CXX)


###########################
# Settings related to g++ #
###########################
# SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall  -I /usr/include/mysql -m64  -O3 -DOS_LINUX  -lmysqlclient -lmysqlcppconn -lpthread -L /usr/lib64/mysql ")
# ICTCLAS50 needs -DOS_LINUX option
SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -std=c++11 -DOS_LINUX")
############################
# Setting for dependencies #
############################
# include_directories for dependencies.
set(MYSQL_INCLUDE_PATH /usr/include/mysql)
set(MYSQL_LINK_PATH /usr/lib64/mysql)

# set(LIB_ROOT ${weibo_ad_filter_SOURCE_DIR}/lib)
set(LIB_ROOT lib)
set(ICTCLAS50_LIB_PATH ${LIB_ROOT}/ICTCLAS50)
set(BOOST_REGEX_LIB_PATH ${LIB_ROOT}/REGEX)
# set(MYSQL_CONNECT_PATH ${LIB_ROOT}/mysql-connector-c++)

#################
# Build runtime #
#################
include_directories(${dataprocess_SOURCE_DIR}/src)
include_directories(${MYSQL_INCLUDE_PATH})
include_directories(${dataprocess_SOURCE_DIR}/lib)
include_directories(${dataprocess_SOURCE_DIR}/src/utils)
include_directories(${ICTCLAS50_LIB_PATH})
link_directories(${MYSQL_LINK_PATH})
include_directories(${dataprocess_SOURCE_DIR}/src/dbdao)
include_directories(${dataprocess_SOURCE_DIR}/src/test)
include_directories(${dataprocess_SOURCE_DIR}/src/gen_feature)
include_directories(${dataprocess_SOURCE_DIR}/src/politic_topics)
include_directories(${dataprocess_SOURCE_DIR}/src/topic_summary)
include_directories(${dataprocess_SOURCE_DIR}/src/bayes)
include_directories(${dataprocess_SOURCE_DIR}/src/singlepass)
include_directories(${dataprocess_SOURCE_DIR}/src/coverage)
# Normal build process should be place here.

################
# Intergration #
################
add_subdirectory(src)

set(EXECUTABLE_OUTPUT_PATH ${dataprocess_SOURCE_DIR}/bin)

add_executable(run_dataprocess src/VirtualProjectMain.cpp)
target_link_libraries(run_dataprocess bayes dboperation connection_pool
	        cluster gentopic test ispolitics topicviewpolictics util
coverage_lib parser_lib textsplit_lib simhash_lib)

