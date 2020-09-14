cmake_minimum_required(VERSION 3.13)
project(smaep_cli)

FetchContent_Declare(
  cli11
  URL https://github.com/CLIUtils/CLI11/archive/v1.8.0.zip)
FetchContent_Populate(cli11)
message(STATUS "cli11 is available in " ${cli11_SOURCE_DIR})

if(SMAEP_CODE_COVERAGE)
  if (CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fprofile-instr-generate -fcoverage-mapping")
  else()
    message(SEND_ERROR "Code coverage is not available for this compiler")
  endif()
endif()


add_executable(smaep_cli ${SOURCE_FILES} "./main.cpp")
target_compile_options(smaep_cli PRIVATE -fprofile-instr-generate -fcoverage-mapping)
target_include_directories(smaep_cli
                           PUBLIC 
                           ${cli11_SOURCE_DIR}/include 
                           ./include)
target_link_libraries(smaep_cli PRIVATE smaep json_source $<IF:$<PLATFORM_ID:Linux>,stdc++fs,>)
