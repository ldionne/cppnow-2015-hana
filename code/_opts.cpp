// Copyright Louis Dionne 2015
// Distributed under the Boost Software License, Version 1.0.

#include <boost/hana.hpp>
namespace hana = boost::hana;


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

auto name = [](auto s) { return hana::make_pair("name", s); };
auto description = [](auto s) { return hana::make_pair("description", s); };
auto default_ = [](auto s) { return hana::make_pair("default", s); };


auto options = option_maker<>{} | banner(
"Lorem ipsum dolor sit amet, consectetur adipisicing elit, sed do eiusmod"
"tempor incididunt ut labore et dolore magna aliqua. Ut enim ad minim veniam,"
"quis nostrud exercitation ullamco laboris nisi ut aliquip ex ea commodo"
"consequat. Duis aute irure dolor in reprehenderit in voluptate velit esse"
"cillum dolore eu fugiat nulla pariatur. Excepteur sint occaecat cupidatat non"
"proident, sunt in culpa qui officia deserunt mollit anim id est laborum."
)
| option(name("ignore"), description("Ignore incorrect values"))
| option(name("iters"), description("Number of iterations"), default_(5))

| option(name("volume"), description("Volume level"), default_(3.0))
| check([](auto opts) {
    if (opts["volume"] < 0)
      opts.die("volume must be non-negative");
  })

| option(name("file"), description("Extra data filename to read in"), hana::type<int>)
| check([](auto opts) {
    if (opts["file"] && !opts["file"].exists())
      opts.die("file must exists");
  })
;



int main(int argc, char *argv[]) {
  // options.parse(argv, argv + argc);
}
