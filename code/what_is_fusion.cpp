// Copyright Louis Dionne 2015
// Distributed under the Boost Software License, Version 1.0.

// sample(what_is_fusion-function)
struct widen {
    long double operator()(long double x) const { return x; }
    long double operator()(double x) const { return x; }
    long double operator()(float x) const { return x; }

    long long operator()(int x) const { return x; }
    long long operator()(long x) const { return x; }
    long long operator()(long long x) const { return x; }

    // etc...
};

long double x = widen{}(3.4);
long long y = widen{}(23);
// end-sample

int main() { }
