// Copyright Louis Dionne 2015
// Distributed under the Boost Software License, Version 1.0.

#include <type_traits>


namespace std {
  template <typename ...>
  using void_t = void;
}

// sample(member-soon)
template <typename T, typename = void>
struct has_xxx
  : std::false_type
{ };

template <typename T>
struct has_xxx<T, std::void_t<decltype(&T::xxx)>>
  : std::true_type
{ };

struct Foo { int xxx; };
static_assert(has_xxx<Foo>::value, "");
// end-sample

int main() { }
