# libawful

> A collection of awful archetypes for testing generic code

## Overview
When writing a generic library, one often needs to test generic components with
types having infrequent characteristics. This is to make sure that a generic
component is not assuming more about its inputs than it should. __libawful__
is a collection of types with unusual properties that can be used to check for
corner cases when implementing generic components. The project contains a single
header, `<awful.hpp>`, which defines all the archetypes.

```c++
#include <awful.hpp>
#include <utility>

template <typename T, typename U>
struct naive_pair {
  T first;
  U second;

  naive_pair() = default;

  template <typename Other, typename = decltype(std::declval<Other>().first),
                            typename = decltype(std::declval<Other>().second)>
  naive_pair(Other&& other)
    : first(std::forward<Other>(other).first),
      second(std::forward<Other>(other).second)
  { }
};

int main() {
  naive_pair<awful::trapconstructible, int> pair;
  auto copy = pair; // ERROR: naive_pair tries to copy-construct its first
                    //        member from a non-const reference to `pair.first`.
}
```

## Compiler requirements
This is a C++14 library, and no effort whatsoever will be made to accommodate
nonconforming compilers.
