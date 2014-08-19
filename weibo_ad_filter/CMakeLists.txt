cmake_minimum_required(VERSION 2.8)
project(weibo_ad_filter CXX)


###########################
# Settings related to g++ #
###########################
# SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall  -I /usr/include/mysql -m64  -O3 -DOS_LINUX  -lmysqlclient -lmysqlcppconn -lpthread -L /usr/lib64/mysql ")
SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -I /usr/include/mysql -m64  -O3 -DOS_LINUX  -lmysqlclient -lmysqlcppconn -lpthread -L /usr/lib64/mysql")

############################
# Setting for dependencies #
############################
# include_directories for dependencies.


#################
# Build runtime #
#################
# Normal build process should be place here.

# Normal build process should be place here.

# add_subdirectory(src/example)

################
# Intergration #
################
add_subdirectory(src)  # utility should be built before everything else.

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
option(test_db "db example." ON)

if(test_example)
  # add_subdirectory(test/test_example)
  add_subdirectory(test/test_db)
endif(test_example)
