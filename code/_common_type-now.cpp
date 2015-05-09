// Copyright Louis Dionne 2015
// Distributed under the Boost Software License, Version 1.0.

#include <boost/hana/ext/std/type_traits.hpp>
#include <boost/hana/ext/std/utility.hpp>
#include <boost/hana/tuple.hpp>
#include <boost/hana/type.hpp>

#include <type_traits>
using namespace boost::hana;


template <typename T>
using void_t = std::conditional_t<true, void, T>;


// sample(common_type-now)
template <typename ...>
auto common_type_impl = nothing;

template <typename T1, typename ...Tn>
auto common_type_impl<T1, Tn...> = monadic_fold<Maybe>(
  tuple_t<Tn...>, type<std::decay_t<T1>>,
  sfinae([](auto t, auto u) -> decltype(
    traits::decay(true ? traits::declval(t) : traits::declval(u))
  ) { return {}; })
);

template <typename ...T>
struct common_type : decltype(common_type_impl<T...>) { };
// end-sample

template <typename ...Ts>
using common_type_t = typename common_type<Ts...>::type;


//////////////////////////////////////////////////////////////////////////////
// Tests
//////////////////////////////////////////////////////////////////////////////
template <typename T, typename = void>
struct has_type : std::false_type { };

template <typename T>
struct has_type<T, void_t<typename T::type>> : std::true_type { };

struct A { }; struct B { }; struct C { };


// Ensure proper behavior in normal cases
static_assert(std::is_same<
    common_type_t<char>,
    char
>{}, "");

static_assert(std::is_same<
    common_type_t<A, A>,
    A
>{}, "");

static_assert(std::is_same<
    common_type_t<char, short, char, short>,
    int
>{}, "");

static_assert(std::is_same<
    common_type_t<char, double, short, char, short, double>,
    double
>{}, "");

static_assert(std::is_same<
    common_type_t<char, short, float, short>,
    float
>{}, "");


// Ensure SFINAE-friendliness
static_assert(!has_type<common_type<>>{}, "");
static_assert(!has_type<common_type<int, void>>{}, "");

int main() { }
