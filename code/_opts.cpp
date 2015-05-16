// Copyright Louis Dionne 2015
// Distributed under the Boost Software License, Version 1.0.

#include <boost/hana.hpp>
#include <string>
namespace hana = boost::hana;


template <typename CharT, CharT ...s>
constexpr auto operator"" _s()
{ return hana::string<s...>; }


template <typename ...Options>
struct option_maker {
  hana::_tuple<Options...> options;
};



template <typename ...Attributes>
struct _option { hana::_tuple<Attributes...> attributes; };

template <typename ...Attributes>
auto option(Attributes ...attributes)
{ return _option<Attributes...>{{attributes...}}; }


template <typename Description>
struct _banner { Description description; };

template <typename Description>
auto banner(Description description)
{ return _banner<Description>{description}; }


template <typename ...Attributes>
struct _check { hana::_tuple<Attributes...> attributes; };

template <typename ...Attributes>
auto check(Attributes ...attributes)
{ return _check<Attributes...>{{attributes...}}; }



template <typename ...Options, typename Option>
constexpr option_maker<Options..., Option> operator|(option_maker<Options...> options, Option opt) {
  return {{hana::append(options.options, opt)}};
}

auto name = [](auto s) { return hana::make_pair("name"_s, s); };
auto description = [](auto s) { return hana::make_pair("description"_s, s); };
auto default_ = [](auto s) { return hana::make_pair("default"_s, s); };


auto options = option_maker<>{} | banner(
"Lorem ipsum dolor sit amet, consectetur adipisicing elit, sed do eiusmod"_s
)
| option(name("ignore"_s), description("Ignore incorrect values"_s))
| option(name("iters"_s), description("Number of iterations"_s), default_(5))

| option(name("volume"_s), description("Volume level"_s), default_(3.0))
| check([](auto opts) {
    if (opts["volume"_s] < 0)
      opts.die("volume must be non-negative");
  })

| option(name("file"_s), description("Extra data filename to read in"_s), hana::type<std::string>)
| check([](auto opts) {
    if (opts["file"_s] && !opts["file"_s].exists())
      opts.die("file must exists");
  })
;


int main(int argc, char *argv[]) {
  // options.parse(argv, argv + argc);
}
