// Copyright Louis Dionne 2015
// Distributed under the Boost Software License, Version 1.0.

#include <boost/hana/integral_constant.hpp>
#include <boost/hana/tuple.hpp>

#include <type_traits>


// sample(type_unification-Type)
template <typename T>
class Type { /* nothing */ };

Type<int> t{};
// end-sample

// sample(type_unification-metafunction)
template <typename T>
constexpr Type<T*> add_pointer(Type<T> const&)
{ return {}; }

template <typename T>
constexpr std::false_type is_pointer(Type<T> const&)
{ return {}; }

template <typename T>
constexpr std::true_type is_pointer(Type<T*> const&)
{ return {}; }
// end-sample

namespace usage {
// sample(type_unification-usage)
Type<int> t{};
auto p = add_pointer(t);
static_assert(is_pointer(p), "");
// end-sample
}

namespace sugar {
// sample(type_unification-sugar)
template <typename T>
constexpr Type<T> type{};

auto t = type<int>;
auto p = add_pointer(t);
static_assert(is_pointer(p), "");
// end-sample
}

namespace citizen {
using namespace boost::hana;
using namespace boost::hana::literals;

// sample(type_unification-first_class)
auto xs = make_tuple(type<int>, type<char>, type<void>);
auto c = xs[1_c];

// sugar:
auto ys = tuple_t<int, char, void>;
// end-sample
}

int main() { }
