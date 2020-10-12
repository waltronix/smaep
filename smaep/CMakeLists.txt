cmake_minimum_required(VERSION 3.13)
project(smaep)

FetchContent_Declare(
  pegtl
  GIT_REPOSITORY https://github.com/taocpp/PEGTL.git
  GIT_TAG 2.8.1)
FetchContent_Populate(PEGTL)
message(STATUS "PEGTL is available in " ${pegtl_SOURCE_DIR})

add_library(smaep INTERFACE)
target_include_directories(smaep INTERFACE ${pegtl_SOURCE_DIR}/include
                                           ./include)

file(GLOB HEADER_FILES ./include/*.hpp)

if(SMAEP_BUILD_TESTS)
  add_executable(smaep_test ${HEADER_FILES} test/test_main.cpp
                            test/test_parser.cpp)
  target_link_libraries(smaep_test PRIVATE smaep)
  target_include_directories(
    smaep_test PRIVATE ${pegtl_SOURCE_DIR}/include
                       ${catch_SOURCE_DIR}/single_include ./include)

  add_test(NAME smaep_test COMMAND smaep_test)
endif()

add_coverage(smaep_test)
