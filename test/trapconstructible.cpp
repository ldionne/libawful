// Copyright Louis Dionne 2016
// Distributed under the Boost Software License, Version 1.0.
// See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt

#include <awful.hpp>

#include <utility>


int main() {
  awful::trapconstructible a;
  awful::trapconstructible b(a);
  awful::trapconstructible c{a};
  awful::trapconstructible d = a;
  awful::trapconstructible e = std::move(a);
}
