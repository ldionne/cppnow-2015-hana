// Copyright Louis Dionne 2015
// Distributed under the Boost Software License, Version 1.0.

#include <boost/hana/tuple.hpp>

#include <iostream>
using namespace boost::hana;


// sample(why-unpack-now)
void f(int a, double b, std::string c) {
  std::cout << a << ":" << b << ":" << c << std::endl;
}

int main() {
  unpack(make_tuple(1, 3.4, "abcdef"), f);
}
// end-sample
