// Copyright Louis Dionne 2015
// Distributed under the Boost Software License, Version 1.0.

#include <boost/hana.hpp>
#include <boost/hana/ext/std/type_traits.hpp>

#include <functional>
#include <string>
#include <type_traits>
#include <typeindex>
#include <typeinfo>
#include <unordered_map>
#include <utility>

#include <boost/any.hpp>
namespace hana = boost::hana;

#include <iostream>

template <typename T>
struct _case {
  template <typename F>
  friend constexpr auto operator>>(_case<T>, F f) {
    return hana::make_pair(hana::type<T>, f);
  }
};

template <typename T>
constexpr _case<T> case_{};


struct _otherwise {
  template <typename F>
  friend constexpr auto operator>>(_otherwise self, F f) {
    return hana::make_pair(hana::type<_otherwise>, f);
  }
};

constexpr _otherwise otherwise{};


auto jump = [](auto ...cases) {
  static auto parts = hana::partition(hana::make_tuple(cases...), [](auto case_) {
    return hana::first(case_) != hana::type<_otherwise>;
  });

  static_assert(!decltype(hana::is_empty(hana::second(parts))){},
  "switch missing a default case");
  static auto default_ = hana::head(hana::second(parts));

  static auto table = hana::unpack(hana::first(parts),
  [=](auto ...non_defaults) {
    // BUG: We're deducing the result type to the wrong thing.
    using Return = std::common_type_t<
      typename decltype(hana::first(non_defaults))::type...
    >;
    using Function = std::function<Return(boost::any&)>;

    auto pair = hana::fuse([](auto t, auto f) {
      using T = typename decltype(t)::type;
      return std::make_pair(
        std::type_index{typeid(T)},
        [=](boost::any& a) -> Return {
          return f(*boost::any_cast<T>(&a));
        }
      );
    });
    return std::unordered_map<std::type_index, Function>{
      pair(non_defaults)...
    };
  });

  return [](boost::any& a) -> decltype(auto) {
    auto it = table.find(std::type_index{a.type()});
    if (it != table.end())
      return it->second(a);
    else
      return hana::second(default_)();
  };
};


auto switch_ = [](auto& a) {
  return [&a](auto ...cases) -> decltype(auto) {
    return jump(cases...)(a);
  };
};



int main() {
  boost::any a = std::string{"abcd"};
  switch_(a)(
    case_<std::string> >> [](auto& s) { return s; }
  , case_<char const*> >> [](auto& s) -> std::string { return s; }
  , case_<char*>       >> [](auto& s) -> std::string { return s; }
  , otherwise          >> []() -> std::string { return {}; }
  );
}
