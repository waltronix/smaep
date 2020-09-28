cmake_minimum_required(VERSION 3.13)
project(smaep)

set(CMAKE_CXX_STANDARD 17)

# set(FETCHCONTENT_BASE_DIR ${CMAKE_SOURCE_DIR}/3rdParty CACHE PATH "Download
# path for 3rdParty stuff")
include(FetchContent)

# force static runtime libraries for msvc builds
if(MSVC)
  set(variables CMAKE_CXX_FLAGS_DEBUG CMAKE_CXX_FLAGS_RELEASE
                CMAKE_CXX_FLAGS_RELWITHDEBINFO CMAKE_CXX_FLAGS_MINSIZEREL)
  foreach(variable ${variables})
    if(${variable} MATCHES "/MD")
      string(REGEX REPLACE "/MD" "/MT" ${variable} "${${variable}}")
    endif()
  endforeach()
endif()

option(SMAEP_BUILD_CLI_TOOL "Build the smaep cli calculator." ON)
option(SMAEP_BUILD_JSON_SOURCE "Build json data source." ON)
option(SMAEP_BUILD_TESTS "Build unit tests for all components." ON)
option(SMAEP_CODE_COVERAGE "Enable clang code coverage for tests." ON)

if(SMAEP_BUILD_TESTS)
  FetchContent_Declare(
    catch
    GIT_REPOSITORY https://github.com/catchorg/Catch2.git
    GIT_TAG v2.9.1)
  FetchContent_Populate(catch)
  message(STATUS "catch is available in " ${catch_SOURCE_DIR})

  enable_testing()
endif()

add_subdirectory("./smaep")

if(SMAEP_BUILD_CLI_TOOL)
  set(SMAEP_BUILD_JSON_SOURCE ON)
endif()

if(SMAEP_BUILD_JSON_SOURCE)
  add_subdirectory("./json_source")
endif()

if(SMAEP_BUILD_CLI_TOOL)
  add_subdirectory("./smaep_cli")
endif()
