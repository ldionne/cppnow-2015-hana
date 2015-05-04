// Copyright Louis Dionne 2015
// Distributed under the Boost Software License, Version 1.0.

// sample(why-unrolling-then)
__attribute__((noinline)) void f() { }

int main() {
    #pragma PLZ UNROLL
    for (int i = 0; i != 10; ++i)
        f();
}
// end-sample
