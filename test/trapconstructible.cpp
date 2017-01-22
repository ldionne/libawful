// Copyright Louis Dionne 2017
// Distributed under the Boost Software License, Version 1.0.
// See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt

#include <awful.hpp>

#include <utility>


int main() {
  awful::trapconstructible const a;
  awful::trapconstructible b;

  awful::trapconstructible copy1(a);
  awful::trapconstructible copy2{a};
  awful::trapconstructible copy3 = a;

  awful::trapconstructible move1(std::move(b));
  awful::trapconstructible move2{std::move(b)};
  awful::trapconstructible move3 = std::move(b);
}
