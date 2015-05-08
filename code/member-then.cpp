// Copyright Louis Dionne 2015
// Distributed under the Boost Software License, Version 1.0.

#include <type_traits>


// sample(member-then)
template <typename T, typename = decltype(&T::xxx)>
static std::true_type has_xxx_impl(int);

template <typename T>
static std::false_type has_xxx_impl(...);

template <typename T>
struct has_xxx
    : decltype(has_xxx_impl<T>(int{}))
{ };

struct Foo { int xxx; };
static_assert(has_xxx<Foo>::value, "");
// end-sample

int main() { }
