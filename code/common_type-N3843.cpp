// Copyright Louis Dionne 2015
// Distributed under the Boost Software License, Version 1.0.

#include <type_traits>


template <typename T>
using void_t = std::conditional_t<true, void, T>;


// sample(common_type-N3843)
template <typename T, typename U>
using builtin_common_t = std::decay_t<decltype(
  true ? std::declval<T>() : std::declval<U>()
)>;

template <typename, typename ...>
struct ct { };

template <typename T>
struct ct<void, T> : std::decay<T> { };

template <typename T, typename U, typename ...V>
struct ct<void_t<builtin_common_t<T, U>>, T, U, V...>
  : ct<void, builtin_common_t<T, U>, V...>
{ };

template <typename ...T>
struct common_type : ct<void, T...> { };
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
