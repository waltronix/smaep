cmake_minimum_required(VERSION 3.16)
project(smaep)

FetchContent_Declare(
  pegtl
  URL https://github.com/taocpp/PEGTL/archive/2.8.3.zip)
FetchContent_Populate(PEGTL)
message(STATUS "PEGTL is available in " ${pegtl_SOURCE_DIR})

file(GLOB HEADERS include/* include/smaep/*)

add_library(smaep STATIC src/smaep.cpp ${HEADERS})
target_include_directories(
  smaep
  PUBLIC ./include
  PRIVATE ${pegtl_SOURCE_DIR}/include)

if(SMAEP_BUILD_TESTS)
  file(GLOB TEST_SRC test/*.cpp)

  add_executable(smaep_test ${TEST_SRC})
  target_link_libraries(smaep_test PRIVATE smaep)
  target_include_directories(
    smaep_test PRIVATE ${pegtl_SOURCE_DIR}/include
                       ${catch_SOURCE_DIR}/single_include ./include)

  add_coverage(smaep_test)
  add_test(NAME smaep_test COMMAND smaep_test)
endif()
