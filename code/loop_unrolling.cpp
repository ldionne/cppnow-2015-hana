// Copyright Louis Dionne 2015
// Distributed under the Boost Software License, Version 1.0.

#include <boost/hana/integral_constant.hpp>
using namespace boost::hana;


namespace then {
// sample(loop_unrolling-then)
__attribute__((noinline)) void f() { }

int main() {
    #pragma PLZ UNROLL
    for (int i = 0; i != 10; ++i)
        f();
}
// end-sample
}

namespace now {
// sample(loop_unrolling-now)
__attribute__((noinline)) void f() { }

int main() {
    int_<10>.times(f);
}
// end-sample
}

int main() { }
