cmake_minimum_required(VERSION 2.8)
project(weibo_ad_filter CXX)


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
set(LIB_ROOT ./lib)
set(ICTCLAS50_LIB_PATH ${LIB_ROOT}/ICTCLAS50)

#################
# Build runtime #
#################
include_directories(${weibo_ad_filter_SOURCE_DIR}/src)
include_directories(./src)
include_directories(${MYSQL_INCLUDE_PATH})
include_directories(${weibo_ad_filter_SOURCE_DIR}/lib)

link_directories(${MYSQL_LINK_PATH})
# Normal build process should be place here.

################
# Intergration #
################
add_subdirectory(src)
file(COPY
	${weibo_ad_filter_SOURCE_DIR}/lib/data_split/Configure.xml
	${weibo_ad_filter_SOURCE_DIR}/lib/data_split/stopwords.txt
	${weibo_ad_filter_SOURCE_DIR}/lib/data_split/user.lic
	${weibo_ad_filter_SOURCE_DIR}/lib/data_split/userdict.txt
	DESTINATION ${weibo_ad_filter_BINARY_DIR}/)
file(COPY
	${weibo_ad_filter_SOURCE_DIR}/lib/data_split/Configure.xml
	${weibo_ad_filter_SOURCE_DIR}/lib/data_split/stopwords.txt
	${weibo_ad_filter_SOURCE_DIR}/lib/data_split/user.lic
	${weibo_ad_filter_SOURCE_DIR}/lib/data_split/userdict.txt
	DESTINATION ${weibo_ad_filter_BINARY_DIR}/test/test_tactic/)
file(COPY
	${weibo_ad_filter_SOURCE_DIR}/lib/data_split/Configure.xml
	${weibo_ad_filter_SOURCE_DIR}/lib/data_split/stopwords.txt
	${weibo_ad_filter_SOURCE_DIR}/lib/data_split/user.lic
	${weibo_ad_filter_SOURCE_DIR}/lib/data_split/userdict.txt
	DESTINATION ${weibo_ad_filter_BINARY_DIR}/test/test_parser/)
file(COPY
	${weibo_ad_filter_SOURCE_DIR}/lib/data_split/Configure.xml
	${weibo_ad_filter_SOURCE_DIR}/lib/data_split/stopwords.txt
	${weibo_ad_filter_SOURCE_DIR}/lib/data_split/user.lic
	${weibo_ad_filter_SOURCE_DIR}/lib/data_split/userdict.txt
	DESTINATION ${weibo_ad_filter_BINARY_DIR}/test/test_simhash/)
add_executable(run src/process.cpp)
target_link_libraries(run model parser tactic connection_pool db_pool simhash db_operation -lpthread)
# add_subdirectory(src/tactic)
# target_link_libraries(run simple_calculator)

####################
# Setup googletest #
####################
add_subdirectory(lib/gtest-1.7.0)
include_directories(${gtest_SOURCE_DIR}/include ${gtest_SOURCE_DIR}) 
enable_testing()

# function that create test cases.
function(add_gtest item)
  target_link_libraries(${item} gtest_main)
  add_test(${item} ${item})
endfunction()


##############
# Test cases #
##############
# Options related to test cases.
# option(test_example "an example." ON)

add_subdirectory(test/test_db)
add_subdirectory(test/test_parser)
add_subdirectory(test/test_tactic)
add_subdirectory(test/test_simhash)
add_subdirectory(test/test_mysql_conn)
