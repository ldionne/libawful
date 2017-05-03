# Copyright Louis Dionne 2017
# Distributed under the Boost Software License, Version 1.0.
# See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt
#
# This file defines the `compile_fail` function, which creates a compilation
# failure test. A compilation failure test is a test that passes if the source
# file fails to compile.

function(compile_fail testname sourcefile)
  add_executable(${testname} "${sourcefile}")
  set_target_properties(${testname} PROPERTIES EXCLUDE_FROM_ALL true
                                               EXCLUDE_FROM_DEFAULT_BUILD true)
  add_test(NAME ${testname}
    COMMAND ${CMAKE_COMMAND} --build "${CMAKE_BINARY_DIR}" --target ${testname} --config $<CONFIGURATION>)
  set_tests_properties(${testname} PROPERTIES WILL_FAIL true)
endfunction()
