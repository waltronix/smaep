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

if(SMAEP_BUILD_TESTS)
  add_executable(smaep_test ${SOURCE_FILES} test/test_main.cpp
                            test/test_parser.cpp)
  target_link_libraries(smaep_test PRIVATE smaep)
  target_include_directories(
    smaep_test PRIVATE ${pegtl_SOURCE_DIR}/include
                       ${catch_SOURCE_DIR}/single_include ./include)

  add_test(NAME smaep_test COMMAND smaep_test)
endif()

function(add_coverage target)
  message("adding code coverage to target: ${target}")

  target_compile_options(${target} PRIVATE -fprofile-instr-generate
                                           -fcoverage-mapping)
  target_link_options(${target} PRIVATE -fprofile-instr-generate
                      -fcoverage-mapping)

  set(no_cover_regex \"\(_deps|test_\)\")

  add_custom_command(
    OUTPUT ${target}.profraw
    COMMAND LLVM_PROFILE_FILE=${target}.profraw ./${target}
    DEPENDS ${target})

  add_custom_command(
    OUTPUT ${target}.profdata
    COMMAND llvm-profdata-10 merge -sparse ${target}.profraw -o
            ${target}.profdata
    MAIN_DEPENDENCY ${target}.profraw)

  add_custom_command(
    OUTPUT cov_html_${target}/index.html
    COMMAND
      llvm-cov-10 show ./${target} -instr-profile=${target}.profdata
      -output-dir=./cov_html_${target} -format=html
      -ignore-filename-regex=${no_cover_regex} -show-instantiations=false
    MAIN_DEPENDENCY ${target}.profdata)

  add_custom_command(
    OUTPUT ${target}.lcov
    COMMAND
      llvm-cov-10 export ./${target} -instr-profile=${target}.profdata
      -format=lcov -ignore-filename-regex=${no_cover_regex} > ${target}.lcov
    MAIN_DEPENDENCY ${target}.profdata)

  add_custom_target(
    cover_${target}
    COMMAND llvm-cov-10 report ./${target} -instr-profile=${target}.profdata
            -ignore-filename-regex=${no_cover_regex}
    COMMAND echo "created test reports"
    DEPENDS cov_html_${target}/index.html ${target}.lcov)

endfunction()

add_coverage(smaep_test)
