// Copyright Louis Dionne 2015
// Distributed under the Boost Software License, Version 1.0.

#include <boost/hana.hpp>
#include <boost/hana/ext/std/type_traits.hpp>
#include <boost/hana/ext/std/utility.hpp>

#include <array>
#include <functional>
#include <iostream>
#include <string>
#include <type_traits>
#include <typeindex>
#include <typeinfo>
#include <utility>

#include <boost/any.hpp>
namespace hana = boost::hana;


template <typename T>
struct _case {
  template <typename F>
  friend constexpr auto operator>>(_case<T>, F f) {
    return hana::make_pair(hana::type<T>, f);
  }
};

template <typename T>
constexpr _case<T> case_{};

struct _otherwise { };
constexpr auto otherwise = case_<_otherwise>;


// Given a function `f` and a type `t`, this function returns the type of the
// `f(x)` expression, where `x` is an object of type `t`.
//
// TODO: How to properly handle references?
auto result_of = [](auto f, auto t) {
  return hana::type<decltype(f(hana::traits::declval(
    hana::traits::add_lvalue_reference(t)
  )))>;
};

auto switch_ = [](boost::any& a) {
  return [&a](auto&& ...cases) -> decltype(auto) {
    static_assert(sizeof...(cases) > 0,
      "invalid usage of switch without any statements");

    auto parts = hana::partition(
      hana::make_tuple(static_cast<decltype(cases)&&>(cases)...),
      [](auto&& c) { return hana::first(c) != hana::type<_otherwise>; }
    );

    static_assert(decltype(hana::length(hana::second(parts)) == hana::size_t<1>){},
      "invalid usage of switch without a default case");
    auto default_ = hana::second(hana::head(hana::second(std::move(parts))));

    return hana::unpack(hana::first(std::move(parts)), [&](auto&& ...cases) {
      auto types = hana::make_tuple(hana::first(cases)...);
      auto functions = hana::make_tuple(
        hana::second(static_cast<decltype(cases)&&>(cases))...
      );

      using Return = std::common_type_t<
        typename decltype(hana::fuse(hana::traits::common_type)(
          hana::zip.with(result_of, functions, types)
        ))::type,
        decltype(default_())
      >;

      using Function = std::function<Return(boost::any&)>;

      auto make_case = [](auto&& f, auto t) {
        using T = typename decltype(t)::type;
        return std::make_pair(
          std::type_index{typeid(T)},
          [f(static_cast<decltype(f)&&>(f))](boost::any& a) -> Return {
            return f(*boost::unsafe_any_cast<T>(&a));
          }
        );
      };

      using Case = std::pair<std::type_index, Function>;
      auto table = hana::unpack(hana::zip(functions, types), [=](auto&& ...c) {
        return std::array<Case, sizeof...(c)>{{
          make_case(hana::head(c), hana::last(c))...
        }};
      });

      std::type_index const& a_index = std::type_index{a.type()};
      for (int i = 0; i < static_cast<int>(sizeof...(cases)); ++i)
        if (table[i].first == a_index)
          return table[i].second(a);

      return default_();
    });
  };
};



int main() {
  boost::any a = 1;
  switch_(a)(
    case_<std::string> >> [](auto& s) -> short     { std::cout << s;  return 1; }
  , case_<char const*> >> [](auto& s) -> int       { std::cout << s;  return 2; }
  , case_<char*>       >> [](auto& s) -> long      { std::cout << s;  return 3; }
  , otherwise          >> []()        -> long long { std::cout << "otherwise";  return 4; }
  );


  a = "abcdef";
  switch_(a)(
    case_<std::string> >> [](auto& s) -> std::string { std::cout << s; return s; }
  , case_<char const*> >> [](auto& s) -> std::string { std::cout << s; return s; }
  , case_<char*>       >> [](auto& s) -> std::string { std::cout << s; return s; }
  , otherwise          >> []()        -> std::string { std::cout << "otherwise"; return {}; }
  );
}
