cmake_minimum_required(VERSION 3.13)
project(smaep_cli)

add_executable(smaep_cli ${SOURCE_FILES} "./main.cpp")
target_link_libraries(smaep_cli PRIVATE smaep)

# if(BUILD_TESTS) file(GLOB TEST_SRC "test/*.cpp")

# add_executable(smaep_test ${SOURCE_FILES} ${TEST_SRC})
# target_include_directories(smaep_test PRIVATE ${pegtl_SOURCE_DIR}/include
# ${jsoncons_SOURCE_DIR}/include ${catch_SOURCE_DIR}/single_include ./include )

# add_test(NAME smaep_test COMMAND smaep_test) endif()
