// Copyright Louis Dionne 2015
// Distributed under the Boost Software License, Version 1.0.

#include <boost/hana/type.hpp>
using namespace boost::hana;


// sample(why-member-should)
auto has_xxx = is_valid([](auto t) -> decltype(t.xxx) {});

struct Foo { int xxx; };
static_assert(has_xxx(type<Foo>), "");
// end-sample

int main() { }
