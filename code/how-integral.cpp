// Copyright Louis Dionne 2015
// Distributed under the Boost Software License, Version 1.0.

#include <type_traits>
using std::integral_constant;


#if 0
// sample(how-integral)
template <typename T, T v>
struct integral_constant { /* usual stuff */ };
// end-sample
#endif

// sample(how-integral)
template <typename V, V v, typename U, U u>
constexpr auto operator+(integral_constant<V, v>, integral_constant<U, u>)
{ return integral_constant<decltype(v + u), v + u>{}; }

template <typename V, V v, typename U, U u>
constexpr auto operator==(integral_constant<V, v>, integral_constant<U, u>)
{ return integral_constant<bool, v == u>{}; }

// ...

static_assert(integral_constant<int, 1>{} + integral_constant<int, 4>{}
                                          ==
                            integral_constant<int, 5>{}
, "");
// end-sample

// sample(how-integral-sugar)
template <int i>
constexpr integral_constant<int, i> int_{};

static_assert(int_<1> + int_<4> == int_<5>, "");
// end-sample


int main() { }
