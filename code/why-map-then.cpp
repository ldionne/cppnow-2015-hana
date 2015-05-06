// Copyright Louis Dionne 2015
// Distributed under the Boost Software License, Version 1.0.

#include <boost/fusion/include/at_key.hpp>
#include <boost/fusion/include/make_map.hpp>

#include <string>
#include <cassert>
using namespace boost::fusion;


int main() {
// sample(why-map-then)
auto map = make_map<char, int, long, float, double, void>(
  "char", "int", "long", "float", "double", "void"
);

std::string i = at_key<int>(map);
assert(i == "int");
// end-sample
}
