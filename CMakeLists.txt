# Copyright Louis Dionne 2017
# Distributed under the Boost Software License, Version 1.0.
# See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt

cmake_minimum_required(VERSION 3.8)
list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")
project(awful LANGUAGES CXX)

add_library(awful INTERFACE)
target_include_directories(awful INTERFACE "$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>"
                                           $<INSTALL_INTERFACE:include>)
target_compile_features(awful INTERFACE cxx_std_14)

install(TARGETS awful EXPORT AwfulConfig)
install(EXPORT AwfulConfig          DESTINATION lib/cmake/awful)
install(FILES  include/awful.hpp    DESTINATION include)

enable_testing()
include(compile_fail)
file(GLOB testfiles test/*.cpp)
foreach(testfile ${testfiles})
  file(RELATIVE_PATH target "${CMAKE_CURRENT_SOURCE_DIR}" "${testfile}")
  string(REPLACE .cpp "" target ${target})
  string(REPLACE / "." target ${target})

  if (${target} MATCHES "[.]fail")
    compile_fail(${target} "${testfile}")
  else()
    add_executable(${target} "${testfile}")
    add_test(${target} ${target})
  endif()

  target_link_libraries(${target} awful)
  set_target_properties(${target} PROPERTIES CXX_EXTENSIONS NO)
endforeach()
