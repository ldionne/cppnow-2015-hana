// Copyright Louis Dionne 2015
// Distributed under the Boost Software License, Version 1.0.

#include <boost/hana.hpp>

#include <iostream>
#include <string>
using namespace boost::hana;


struct concat_strings {
  template <char ...s1, char ...s2>
  constexpr auto operator()(_string<s1...>, _string<s2...>) const
  { return string<s1..., s2...>; }
};

template <typename S>
constexpr auto concat_all(S s) {
  return fold.left(s, string<>, concat_strings{});
}



constexpr auto formats = make_map(
  make_pair(type<int>, string<'%', 'd'>),
  make_pair(type<double>, string<'%', 'f'>),
  make_pair(type<char const*>, string<'%', 's'>)
);

constexpr struct _done { } done{};

template <typename F, typename ...Tokens>
struct unstream_t {
  F f;
  _tuple<Tokens...> tokens;

  template <typename Token>
  friend constexpr
  unstream_t<F, Tokens..., std::decay_t<Token>>
  operator<<(unstream_t&& self, Token&& token) {
    return {
      std::move(self.f),
      append(std::move(self.tokens), std::forward<Token>(token))
    };
  }

  friend auto operator<<(unstream_t&& self, _done) {
    auto fmt = concat_all(
      adjust_if(self.tokens,
        compose(not_, is_a<String>),
        [](auto&& token) {
          return formats[decltype_(token)];
        }
      )
    );

    auto args = remove_if(std::move(self.tokens), is_a<String>);

    return unpack(std::move(args), [&](auto&& ...args) {
      return std::move(self.f)(
        to<char const*>(fmt),
        std::forward<decltype(args)&&>(args)...
      );
    });
  }
};

template <typename F>
constexpr unstream_t<std::decay_t<F>> unstream(F&& f) {
    return {std::forward<F>(f), {}};
}

int main() {
  unstream(std::printf)
    << BOOST_HANA_STRING("C++Now ")
    << 2015
    << " is"
    << BOOST_HANA_STRING(" awesome!")
    << done;

  // equivalent to

  std::printf("C++Now %i%s awesome!", 2015, " is");
}
