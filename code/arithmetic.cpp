// Copyright Louis Dionne 2015
// Distributed under the Boost Software License, Version 1.0.

#include <boost/mpl/equal_to.hpp>
#include <boost/mpl/int.hpp>
#include <boost/mpl/integral_c.hpp>
#include <boost/mpl/minus.hpp>
#include <boost/mpl/multiplies.hpp>
#include <boost/mpl/pair.hpp>
#include <boost/mpl/plus.hpp>

#include <boost/hana/constant.hpp>
#include <boost/hana/core/models.hpp>
#include <boost/hana/integral_constant.hpp>
#include <boost/hana/pair.hpp>

#include <cassert>
#include <type_traits>
namespace hana = boost::hana;


template <typename T, typename = std::enable_if_t<
  !hana::models<hana::Constant, T>()
>>
constexpr T sqrt(T x) {
  T inf = 0, sup = (x == 1 ? 1 : x/2);
  while (!((sup - inf) <= 1 || ((sup*sup <= x) && ((sup+1)*(sup+1) > x)))) {
    T mid = (inf + sup) / 2;
    bool take_inf = mid*mid > x ? 1 : 0;
    inf = take_inf ? inf : mid;
    sup = take_inf ? mid : sup;
  }

  return sup*sup <= x ? sup : inf;
}

template <typename T, typename = std::enable_if_t<
  hana::models<hana::Constant, T>()
>>
constexpr auto sqrt(T const&) {
  return hana::integral_constant<typename T::value_type, sqrt(T::value)>;
}


namespace then {
using namespace boost::mpl;

template <typename N>
struct sqrt
  : integral_c<typename N::value_type, ::sqrt(N::value)>
{ };

template <typename X, typename Y>
using point = pair<X, Y>;

// sample(arithmetic-then)
template <typename P1, typename P2>
struct distance {
  using xs = typename minus<typename P1::first,
                            typename P2::first>::type;
  using ys = typename minus<typename P1::second,
                            typename P2::second>::type;
  using type = typename sqrt<
    typename plus<
      typename multiplies<xs, xs>::type,
      typename multiplies<ys, ys>::type
    >::type
  >::type;
};

static_assert(equal_to<
  distance<point<int_<3>, int_<5>>, point<int_<7>, int_<2>>>::type,
  int_<5>
>::value, "");
// end-sample
}


namespace now {
using namespace boost::hana;
using namespace boost::hana::literals;

constexpr auto point = make_pair;

// sample(arithmetic-now)
template <typename P1, typename P2>
constexpr auto distance(P1 p1, P2 p2) {
  auto xs = first(p1) - first(p2);
  auto ys = second(p1) - second(p2);
  return sqrt(xs*xs + ys*ys);
}

static_assert(distance(point(3_c, 5_c), point(7_c, 2_c)) == 5_c, "");
// end-sample

void test() {

// sample(arithmetic-now-dynamic)
auto p1 = point(3, 5); // dynamic values now
auto p2 = point(7, 2); //
assert(distance(p1, p2) == 5); // same function works!
// end-sample

}
}


int main() {
  now::test();
}
