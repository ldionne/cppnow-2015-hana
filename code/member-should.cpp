// Copyright Louis Dionne 2015
// Distributed under the Boost Software License, Version 1.0.

#include <boost/hana/type.hpp>
using namespace boost::hana;


int main() {
// sample(member-should)
auto has_xxx = is_valid([](auto t) -> decltype(t.xxx) {});

struct Foo { int xxx; };
Foo foo{1};

static_assert(has_xxx(foo), "");
static_assert(!has_xxx("abcdef"), "");
// end-sample
}
