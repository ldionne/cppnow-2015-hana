// Copyright Louis Dionne 2015
// Distributed under the Boost Software License, Version 1.0.

#include <boost/hana/map.hpp>
#include <boost/hana/type.hpp>

#include <string>
#include <cassert>
using namespace boost::hana;


int main() {
// sample(why-map-now)
auto map = make_map(
  make_pair(type<char>,   "char"),
  make_pair(type<int>,    "int"),
  make_pair(type<long>,   "long"),
  make_pair(type<float>,  "float"),
  make_pair(type<double>, "double")
);

std::string i = map[type<int>];
assert(i == "int");
// end-sample
}
