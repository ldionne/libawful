// Sole header file implementing the `awful` library of archetypes for testing
// generic code. For documentation, questions, bug reports and feature requests,
// see https://github.com/ldionne/awful.
//
// Copyright Louis Dionne 2016
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

// A (literal) type that can only be default constructed and moved around
// through move-construction and move-assignment.
//
// It is useful for making sure that an algorithm does not try to
// copy objects around.
struct noncopyable {
  constexpr noncopyable() = default;
  constexpr noncopyable(noncopyable const&) = delete;
  constexpr noncopyable(noncopyable &&) = default;

  constexpr noncopyable& operator=(noncopyable const&) = delete;
  constexpr noncopyable& operator=(noncopyable &&) = default;
};

// A type that tracks its current state (constructed, moved-from, destroyed, etc.)
// and asserts that it's not used in an invalid state.
//
// It can be used to check for double-moves, double-deletes and other similar
// lifetime issues.
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

} // end namespace awful

#endif // !AWFUL_HPP
