// Copyright Louis Dionne 2015
// Distributed under the Boost Software License, Version 1.0.

#include <boost/hana/tuple.hpp>

#include <tuple>
namespace hana = boost::hana;
using namespace hana::literals;


int main() {

{

// sample(why-nth-then)
std::tuple<int, char, float> values = {1, 'x', 3.4f};
char x = std::get<1>(values);
// end-sample
(void)x;

}{

// sample(why-nth-now)
hana::_tuple<int, char, float> values = {1, 'x', 3.4f};
char x = values[1_c];
// end-sample
(void)x;

}

}
