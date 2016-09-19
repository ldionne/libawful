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
