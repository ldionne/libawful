// Copyright Louis Dionne 2016
// Distributed under the Boost Software License, Version 1.0.
// See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt

#include <awful.hpp>


int main() {
  awful::noncopyable a;
  awful::noncopyable b = a;
}
