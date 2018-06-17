cmake_minimum_required(VERSION 2.8)
set(CMAKE_CXX_STANDARD 14)

project(BIGINT)

include_directories(${BIGINT_SOURCE_DIR})

add_executable(big_integer_testing
        big_integer_testing.cpp
        big_integer.h
        big_integer.cpp
        gtest/gtest-all.cc
        gtest/gtest.h
        gtest/gtest_main.cc vector/socow_vector.cpp vector/socow_vector.h)

if(CMAKE_COMPILER_IS_GNUCC OR CMAKE_COMPILER_IS_GNUCXX)
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -pedantic")
  set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -ggdb -fsanitize=address,undefined -D_GLIBCXX_DEBUG")
  set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -O3 -D_GLIBCXX")
endif()

target_link_libraries(big_integer_testing -lpthread)

