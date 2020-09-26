cmake_minimum_required(VERSION 3.16)
project(smaep)

FetchContent_Declare(pegtl
                     GIT_REPOSITORY https://github.com/taocpp/PEGTL.git
                     GIT_TAG 2.8.1)
FetchContent_Populate(PEGTL)
message(STATUS "PEGTL is available in " ${pegtl_SOURCE_DIR})

add_library(smaep INTERFACE)
target_include_directories(smaep
                           INTERFACE ${pegtl_SOURCE_DIR}/include ./include)

if(SMAEP_BUILD_TESTS)
  add_executable(smaep_test ${SOURCE_FILES} test/test_main.cpp test/test_parser.cpp)
  target_link_libraries(smaep_test PRIVATE smaep)
  target_include_directories(smaep_test
                             PRIVATE ${pegtl_SOURCE_DIR}/include
                                     ${catch_SOURCE_DIR}/single_include
                                     ./include)                                   
  add_code_coverage(smaep_test)

  add_test(NAME smaep_test COMMAND smaep_test)
endif()

set(exe_path ${CMAKE_CURRENT_SOURCE_DIR}/smaep_test)

add_custom_command(OUTPUT smaep_test.profraw
                   COMMAND LLVM_PROFILE_FILE=smaep_test.profraw ./smaep_test
                   DEPENDS smaep_test)

add_custom_command(OUTPUT smaep_test.profdata
                   COMMAND llvm-profdata-10 merge -sparse smaep_test.profraw -o smaep_test.profdata
                   MAIN_DEPENDENCY smaep_test.profraw)

add_custom_command(OUTPUT cov_html/index.html
                   COMMAND llvm-cov-10 show ./smaep_test -instr-profile=smaep_test.profdata -output-dir=./cov_html -format=html -ignore-filename-regex="\(_deps|test_\)" -show-instantiations=false
                   MAIN_DEPENDENCY smaep_test.profdata)

add_custom_command(OUTPUT smaep_test.lcov
                   COMMAND llvm-cov-10 export ./smaep_test -instr-profile=smaep_test.profdata -format=lcov -ignore-filename-regex="\(_deps|test_\)" > smaep_test.lcov
                   MAIN_DEPENDENCY smaep_test.profdata)

add_custom_target(cover_smaep_test 
                  COMMAND echo "something to show?"
                  DEPENDS cov_html/index.html smaep_test.lcov)