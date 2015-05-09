// Copyright Louis Dionne 2015
// Distributed under the Boost Software License, Version 1.0.

#include <boost/hana/integral_constant.hpp>
using namespace boost::hana;


// sample(unrolling-now)
__attribute__((noinline)) void f() { }

int main() {
    int_<10>.times(f);
}
// end-sample
