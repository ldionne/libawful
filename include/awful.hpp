// This header is part of the libawful project, a collection of archetypes for
// testing generic C++ libraries. For documentation, questions, bug reports and
// feature requests, see https://github.com/ldionne/awful.
//
// Copyright Louis Dionne 2017
// Distributed under the Boost Software License, Version 1.0.
// See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt

#ifndef AWFUL_HPP
#define AWFUL_HPP

#include <cassert>
#include <iosfwd>

#if defined(AWFUL_VERBOSE)
# include <iostream>
#endif


namespace awful {

//////////////////////////////////////////////////////////////////////////////
// What's in this header
//////////////////////////////////////////////////////////////////////////////
// A (literal) type that can only be default constructed and moved around
// through move-construction and move-assignment.
//
// It is useful for making sure that an algorithm does not try to
// copy objects around.
struct noncopyable;

// A type that tracks its current state (constructed, moved-from, destroyed, etc.)
// and asserts that it's not used in an invalid state.
//
// It can be used to check for double-moves, double-deletes and other similar
// lifetime issues.
class tracked;

// A (literal) type that can be default, move and copy constructed, but that
// must not be constructed from anything else. If one tries to construct an
// object of this type with anything, a compilation error is triggered.
//
// This is done by defining a catch-all implicit constructor that triggers
// a `static_assert` whenever instantiated. This can be used to test things
// like making sure that `std::tuple`'s copy constructor only calls the
// copy constructor of its elements, and nothing else. In particular, one
// corner case that may arise is that the copied-from element is not const,
// and so the catch-all constructor of `trapconstructible` will be considered
// a better match than its copy-constructor, which takes a `const&`.
struct trapconstructible;

// A (literal) type overloading the comma operator in a nasty way. Instantiating
// the comma operator of this type causes a compilation error.
//
// This can be used to test that generic algorithms, especially in a
// metaprogramming setting, guard their uses of the comma operator with
// `(void)expr1, expr2`.
struct trapcomma;

//////////////////////////////////////////////////////////////////////////////
// Implementation
//////////////////////////////////////////////////////////////////////////////
struct noncopyable {
  constexpr noncopyable() = default;
  constexpr noncopyable(noncopyable const&) = delete;
  constexpr noncopyable(noncopyable &&) = default;

  constexpr noncopyable& operator=(noncopyable const&) = delete;
  constexpr noncopyable& operator=(noncopyable &&) = default;
};

class tracked {
  enum class State { CONSTRUCTED, MOVED_FROM, DESTROYED };
  int value;
  State state;

public:
  explicit tracked() : tracked{0} { }

  explicit tracked(int k) : value{k}, state{State::CONSTRUCTED} {
#if defined(AWFUL_VERBOSE)
    std::cerr << "constructing " << *this << '\n';
#endif
  }

  tracked(tracked const& t) : value{t.value}, state{State::CONSTRUCTED} {
    assert(t.state != State::MOVED_FROM && "copying a moved-from object");
    assert(t.state != State::DESTROYED && "copying a destroyed object");

#if defined(AWFUL_VERBOSE)
    std::cerr << "copying " << *this << '\n';
#endif
  }

  tracked(tracked&& t) : value{t.value}, state{State::CONSTRUCTED} {
    assert(t.state != State::MOVED_FROM && "double moving from an object");
    assert(t.state != State::DESTROYED && "moving from a destroyed object");

#if defined(AWFUL_VERBOSE)
    std::cerr << "moving " << t << '\n';
#endif
    t.state = State::MOVED_FROM;
  }

  tracked& operator=(tracked const& other) {
    assert(this->state != State::DESTROYED && "assigning to a destroyed object");
    assert(other.state != State::MOVED_FROM && "assigning a moved-from object");
    assert(other.state != State::DESTROYED && "assigning a destroyed object");

#if defined(AWFUL_VERBOSE)
    std::cerr << "assigning " << other << " to " << *this << '\n';
#endif
    this->value = other.value;
    return *this;
  }

  tracked& operator=(tracked&& other) {
    assert(this->state != State::DESTROYED && "assigning to a destroyed object");
    assert(other.state != State::MOVED_FROM && "double-moving from an object");
    assert(other.state != State::DESTROYED && "assigning a destroyed object");

#if defined(AWFUL_VERBOSE)
    std::cerr << "assigning " << other << " to " << *this << '\n';
#endif
    this->value = other.value;
    other.state = State::MOVED_FROM;
    return *this;
  }

  ~tracked() {
    assert(state != State::DESTROYED && "double-destroying an object");

#if defined(AWFUL_VERBOSE)
    std::cerr << "destructing " << *this << '\n';
#endif
    state = State::DESTROYED;
  }

  template <typename CharT, typename Traits>
  friend std::basic_ostream<CharT, Traits>&
  operator<<(std::basic_ostream<CharT, Traits>& os, tracked const& t) {
    os << "tracked{" << t.value << "}";
    switch (t.state) {
    case State::CONSTRUCTED:
      os << "[ok]"; break;
    case State::MOVED_FROM:
      os << "[moved from]"; break;
    case State::DESTROYED:
      os << "[destroyed]"; break;
    default:
      assert(false && "never reached");
    }
    return os;
  }
};

namespace detail {
  template <typename ...>
  struct always_false { static constexpr bool value = false; };
}

struct trapconstructible {
  constexpr trapconstructible() { }
  constexpr trapconstructible(trapconstructible const&) = default;
  constexpr trapconstructible(trapconstructible&&) = default;

  template <typename ...X>
  constexpr trapconstructible(X&&...) {
    static_assert(detail::always_false<X...>::value,
      "this constructor must not be instantiated");
  }
};

struct trapcomma {
  template <typename T>
  friend constexpr void operator,(trapcomma const&, T&&) {
    static_assert(detail::always_false<T>::value,
      "trapcomma::operator, must never be instantiated");
  }

  template <typename T>
  friend constexpr void operator,(T&&, trapcomma const&) {
    static_assert(detail::always_false<T>::value,
      "trapcomma::operator, must never be instantiated");
  }
};

} // end namespace awful

#endif // !AWFUL_HPP
