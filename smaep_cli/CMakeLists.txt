cmake_minimum_required(VERSION 3.16)
project(smaep_cli)

FetchContent_Declare(
  cli11
  URL https://github.com/CLIUtils/CLI11/archive/v1.9.1.zip)
FetchContent_Populate(cli11)
message(STATUS "cli11 is available in " ${cli11_SOURCE_DIR})

add_executable(smaep_cli ${SOURCE_FILES} "./main.cpp")
target_include_directories(smaep_cli
                           PUBLIC 
                           ${cli11_SOURCE_DIR}/include 
                           ./include)
target_link_libraries(smaep_cli PRIVATE smaep json_source $<IF:$<PLATFORM_ID:Linux>,stdc++fs,>)
