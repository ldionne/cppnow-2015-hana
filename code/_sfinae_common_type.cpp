// Copyright Louis Dionne 2015
// Distributed under the Boost Software License, Version 1.0.

#include <boost/hana/ext/std/type_traits.hpp>
#include <boost/hana/ext/std/utility.hpp>
#include <boost/hana/tuple.hpp>
#include <boost/hana/type.hpp>

#include <meta/meta.hpp>

#include <type_traits>


template <typename T>
using void_t = std::conditional_t<true, void, T>;

//////////////////////////////////////////////////////////////////////////////
// Eric Niebler's meta
//////////////////////////////////////////////////////////////////////////////
namespace _meta {
using namespace meta;

// sample(sfinae_common_type-meta)
template <typename T, typename U>
using builtin_common_t = std::decay_t<decltype(
  true ? std::declval<T>() : std::declval<U>()
)>;

template <typename T, typename U>
using lazy_builtin_common_t = defer<builtin_common_t, T, U>;

template <typename ...Ts>
struct common_type { };

template <typename ...Ts>
using common_type_t = eval<common_type<Ts...>>;

template <typename T>
struct common_type<T> : std::decay<T> { };

template <typename T, typename U>
struct common_type<T, U>
  : if_c<(std::is_same<std::decay_t<T>, T>::value &&
          std::is_same<std::decay_t<U>, U>::value),
      lazy::let<lazy_builtin_common_t<T, U>>,
      common_type<std::decay_t<T>, std::decay_t<U>>
    >
{ };

template<typename T, typename U, typename ...Vs>
struct common_type<T, U, Vs...>
  : lazy::let<lazy::fold<list<U, Vs...>, T, quote<common_type_t>>>
{ };
// end-sample
}

//////////////////////////////////////////////////////////////////////////////
// Proposed implementation in n3843 (broken)
//////////////////////////////////////////////////////////////////////////////
namespace n3843 {
// sample(sfinae_common_type-n3843)
template <typename T, typename U>
using builtin_common_t = decltype(
  true ? std::declval<T>() : std::declval<U>()
);

template <typename, typename ...>
struct ct { };

template <typename T>
struct ct<void, T>
  : std::decay<T>
{ };

template <typename T, typename U, typename ...V>
struct ct<void_t<builtin_common_t<T, U>>, T, U, V...>
  : ct<void, builtin_common_t<T, U>, V...>
{ };

template <typename ...T>
struct common_type
  : ct<void, T...>
{ };
// end-sample
}

//////////////////////////////////////////////////////////////////////////////
// Hana
//////////////////////////////////////////////////////////////////////////////
namespace _hana {
using namespace boost::hana;

#if 0
template <typename T, typename U>
struct common_type<T, U>
  : std::conditional_t<std::is_same<std::decay_t<T>, T>{} &&
                       std::is_same<std::decay_t<U>, U>{},
      decltype(builtin_common_t(type<T>, type<U>)),
      common_type<std::decay_t<T>, std::decay_t<U>>
  >
{ };
#endif

// sample(sfinae_common_type-hana)
auto builtin_common_t = sfinae([](auto t, auto u) -> decltype(type<
  std::decay_t<decltype(true ? traits::declval(t) : traits::declval(u))>
>) { return {}; });

template <typename ...T>
struct common_type { };

template <typename T, typename U>
struct common_type<T, U>
  : decltype(builtin_common_t(type<T>, type<U>))
{ };

template <typename T1, typename ...Tn>
struct common_type<T1, Tn...>
  : decltype(monadic_fold<Maybe>(tuple_t<Tn...>,
      type<std::decay_t<T1>>,
      sfinae(metafunction<common_type>)
    ))
{ };
// end-sample
}


using _meta::common_type;
// using n3843::common_type;
// using _hana::common_type;

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


// Ensure common_type respects user specializations
template <> struct common_type<A, B> { using type = C; };
template <> struct common_type<B, A> { using type = C; };
template <> struct common_type<B, C> { using type = C; };
template <> struct common_type<C, B> { using type = C; };
template <> struct common_type<A, C> { using type = C; };
template <> struct common_type<C, A> { using type = C; };
static_assert(std::is_same<
    common_type_t<A, B>,
    C
>{}, "");

static_assert(std::is_same<
    common_type_t<B, A>,
    C
>{}, "");

static_assert(std::is_same<
  common_type_t<A, B, B>,
  C
>{}, "");

int main() { }
