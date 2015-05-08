// Copyright Louis Dionne 2015
// Distributed under the Boost Software License, Version 1.0.

#include <boost/mpl/less.hpp>
#include <boost/mpl/min_element.hpp>
#include <boost/mpl/placeholders.hpp>
#include <boost/mpl/sizeof.hpp>
#include <boost/mpl/vector.hpp>

#include <boost/hana/tuple.hpp>
#include <boost/hana/type.hpp>

#include <type_traits>


template <int n>
struct storage { char weight[n]; };

namespace then {
using namespace boost::mpl;

// sample(smallest_type-then)
template <typename ...T>
struct smallest
  : deref<
    typename min_element<
      vector<T...>, less<sizeof_<_1>, sizeof_<_2>>
    >::type
  >
{ };

template <typename ...T>
using smallest_t = typename smallest<T...>::type;

static_assert(std::is_same<
  smallest_t<char, long, long double>, char
>::value, "");
// end-sample

static_assert(std::is_same<
  smallest_t<storage<3>, storage<1>, storage<2>>,
  storage<1>
>::value, "");
}


namespace now {
using namespace boost::hana;

// sample(smallest_type-now)
template <typename ...T>
auto smallest = minimum(tuple_t<T...>, [](auto t, auto u) {
  return sizeof_(t) < sizeof_(u);
});

template <typename ...T>
using smallest_t = typename decltype(smallest<T...>)::type;

static_assert(std::is_same<
  smallest_t<char, long, long double>, char
>::value, "");
// end-sample

static_assert(std::is_same<
  smallest_t<storage<3>, storage<1>, storage<2>>,
  storage<1>
>::value, "");
}

int main() { }
