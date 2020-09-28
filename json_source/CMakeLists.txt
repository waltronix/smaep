cmake_minimum_required(VERSION 3.13)
project(json_source)

FetchContent_Declare(
  jsoncons
  GIT_REPOSITORY https://github.com/danielaparker/jsoncons.git
  GIT_TAG v0.140.0)
FetchContent_Populate(jsoncons)
message(STATUS "jsoncons is available in " ${jsoncons_SOURCE_DIR})

add_library(json_source STATIC src/json_source.cpp)
target_include_directories(json_source PUBLIC ${jsoncons_SOURCE_DIR}/include
                                              ./include)
target_link_libraries(json_source PUBLIC smaep)

if(BUILD_TESTS)
  file(GLOB TEST_SRC "test/*.cpp")

  add_executable(json_source_test ${TEST_SRC})
  target_link_libraries(json_source_test PRIVATE json_source)
  target_include_directories(json_source_test
                             PRIVATE ${catch_SOURCE_DIR}/single_include)

  add_test(NAME json_source_test COMMAND json_source_test)
endif()
