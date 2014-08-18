cmake_minimum_required(VERSION 2.8)
project(weibo_analyse CXX)


###########################
# Settings related to g++ #
###########################
SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -std=c++11")


############################
# Setting for dependencies #
############################
# include_directories for dependencies.


#################
# Build runtime #
#################
# Search files from ${qrobot_SOURCE_DIR}/src.
include_directories(src)

# Normal build process should be place here.
add_subdirectory(src/example)  # utility should be built before everything else.


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
