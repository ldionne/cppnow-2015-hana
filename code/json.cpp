// Copyright Louis Dionne 2015
// Distributed under the Boost Software License, Version 1.0.

#include <boost/hana.hpp>
#include <boost/hana/struct_macros.hpp>

#include <iostream>
#include <string>
#include <type_traits>
#include <utility>
using namespace boost::hana;
using namespace std::literals;




template <typename Xs>
std::string join(Xs&& xs, std::string sep) {
  return fold.left(intersperse(std::forward<Xs>(xs), sep), "", _ + _);
}

// sample(json-base)
std::string quote(std::string s) { return "\"" + s + "\""; }

template <typename T>
auto to_json(T&& x) -> decltype(std::to_string(std::forward<T>(x))) {
  return std::to_string(std::forward<T>(x));
}

std::string to_json(char c) { return quote({c}); }
std::string to_json(std::string s) { return quote(s); }
// end-sample

// sample(json-Struct)
template <typename T>
std::enable_if_t<models<Struct, T>(), std::string> to_json(T&& x) {
  auto as_tuple = to<Tuple>(std::forward<T>(x));

  auto json = transform(std::move(as_tuple),
    fuse([](auto name, auto&& member) {
      return quote(to<char const*>(name)) + " : " +
                to_json(std::forward<decltype(member)>(member));
    }));

  return "{" + join(std::move(json), ", ") + "}";
}
// end-sample

// sample(json-Sequence)
template <typename Xs>
std::enable_if_t<models<Sequence, Xs>(), std::string> to_json(Xs&& xs) {
  auto json = transform(std::forward<Xs>(xs), [](auto&& x) {
    return to_json(std::forward<decltype(x)>(x));
  });

  return "[" + join(std::move(json), ", ") + "]";
}
// end-sample


int main() {
// sample(json-usage)
struct Person {
  BOOST_HANA_DEFINE_STRUCT(Person,
    (std::string, name),
    (int, age)
  );
};

Person joe{"Joe", 30};
std::cout << to_json(make_tuple(1, 'c', joe));
// end-sample
}
