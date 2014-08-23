cmake_minimum_required(VERSION 2.8)
project(weibo_models CXX)


###########################
# Settings related to g++ #
###########################
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -std=c++11")
add_definitions(-DLOGGER_LEVEL=LL_WARN)


############################
# Setting for dependencies #
############################
# Globals path for CMakeLists.txt of sub-directories.
set(LIB_ROOT ${weibo_models_SOURCE_DIR}/lib)
set(MYSQL_CONNECT_PATH ${LIB_ROOT}/mysql-connector-c++)
set(CPPJIEBA ${LIB_ROOT}/libcppjieba)

# Added headers to search path.
include_directories(${MYSQL_CONNECT_PATH}/include)
include_directories(${CPPJIEBA}/include)

# required dataset.
file(INSTALL ${CPPJIEBA}/dict/jieba.dict.utf8
             ${CPPJIEBA}/dict/hmm_model.utf8
             ${CPPJIEBA}/dict/idf.utf8
             ${CPPJIEBA}/dict/stop_words.utf8
     DESTINATION data/jieba)


#################
# Build runtime #
#################
include_directories(src)

# Normal build process should be place here.
add_subdirectory(src/example)
add_subdirectory(src/database)
add_subdirectory(src/utils)
add_subdirectory(src/data_mining)


################
# Intergration #
################
add_executable(run src/main.cc)
target_link_libraries(run simple_calculator)


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
option(test_example "an example." ON)

if(test_example)
  add_subdirectory(test/test_example)
endif(test_example)

add_subdirectory(test/test_mysql_conn)
add_subdirectory(test/test_utils)
add_subdirectory(test/test_data_mining)
