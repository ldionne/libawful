// Copyright Louis Dionne 2016
// Distributed under the Boost Software License, Version 1.0.
// See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt

#include <awful.hpp>


int f() { return 0; }

int main() {
  awful::trapcomma a;
  (void)a, 0;
  f(), (void)a; // avoid 'expression result unused' warning
}
