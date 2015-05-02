// Copyright Louis Dionne 2015
// Distributed under the Boost Software License, Version 1.0.

#include <boost/hana/type.hpp>

#include <type_traits>
using namespace boost::hana;


namespace then {
// sample(checking_for_member-then)
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
}

namespace now {
// sample(checking_for_member-now)
auto has_xxx = is_valid([](auto t) -> decltype(t.xxx) {});

struct Foo { int xxx; };
static_assert(has_xxx(type<Foo>), "");
// end-sample
}

int main() { }