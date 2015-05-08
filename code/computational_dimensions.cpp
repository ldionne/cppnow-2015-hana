// Copyright Louis Dionne 2015
// Distributed under the Boost Software License, Version 1.0.

#include <boost/fusion/include/find_if.hpp>
#include <boost/fusion/include/vector.hpp>

#include <boost/mpl/equal.hpp>
#include <boost/mpl/placeholders.hpp>
#include <boost/mpl/transform.hpp>
#include <boost/mpl/vector.hpp>

#include <array>
#include <cassert>
#include <string>
#include <tuple>
#include <type_traits>
#include <vector>
namespace fusion = boost::fusion;
namespace mpl = boost::mpl;
using mpl::_1;


int main() {

{

// sample(dimensions-homo_dynamic)
std::vector<int> seq{1, 2, 3, 4, 5};
std::transform(seq.begin(), seq.end(), seq.begin(), [](int i) {
  return i * i;
});

std::vector<int> squares{1, 4, 9, 16, 25};
assert(seq == squares);
// end-sample

}{

std::vector<int> squares{1, 4, 9, 16, 25};

// sample(dimensions-hetero_dynamic)
std::tuple<std::string, std::vector<int>> seq{"abcd", squares};
std::string& s = std::get<0>(seq);
assert(s == "abcd");

fusion::vector<std::string, int, float, long> v = {"abc", 2, 3.4f, 5l};
assert(*fusion::find_if<std::is_integral<mpl::_1>>(v) == 2);
// end-sample

}{

// sample(dimensions-hetero_constexpr)
constexpr std::tuple<int, char, float> seq = {1, 'c', 3.4f};
constexpr char c = std::get<1>(seq);
static_assert(c == 'c', "");
// end-sample

}

}

namespace homo_constexpr {
// sample(dimensions-homo_constexpr)
constexpr long fact(long n) {
  return n == 0 ? 1 : n * fact(n - 1);
}

constexpr std::array<long,4> seq{{fact(1), fact(2), fact(3), fact(4)}};
static_assert(seq[3] == 24, "");
// end-sample
}


namespace type_level {
// sample(dimensions-type_level)
using seq = mpl::vector<int, char, float>;
using ptrs = mpl::transform<seq, std::add_pointer<mpl::_1>>::type;

static_assert(mpl::equal<
  ptrs, mpl::vector<int*, char*, float*>
>::value, "");
// end-sample
}

