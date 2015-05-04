// Copyright Louis Dionne 2015
// Distributed under the Boost Software License, Version 1.0.

#include <boost/hana.hpp>
#include <boost/hana/ext/std/type_traits.hpp>

#include <type_traits>
using namespace boost::hana;
using namespace boost::hana::literals;


// sample(how-printf-formats)
auto formats = make_map(
  make_pair(char_<'i'>, type<int>),
  make_pair(char_<'f'>, type<double>),
  make_pair(char_<'s'>, type<char*>)

  // ...
);
// end-sample

// sample(how-printf-core)
template <typename Fmt, typename ...Args>
int type_safe_printf(Fmt fmt, Args const& ...args) {
  static_assert(is_a<String>(fmt),
  "the format string must be a compile-time hana::String");

  // type-checking is done here, and then we just forward to printf.
  auto format_chars = filter(to<Tuple>(fmt), [](auto c) {
    return c ^in^ formats;
  });

  static_assert(length(format_chars) == sizeof...(args),
  "the number of format characters does not match the number of arguments");

  auto conversions = zip(tuple_t<std::decay_t<Args>...>, format_chars);
  for_each(conversions, fuse([](auto arg_type, auto format_char) {
    static_assert(decltype(formats[format_char] == arg_type){},
    "the type of the argument does not match the format character");
  }));

  return std::printf(to<char const*>(fmt), args...);
};
// end-sample

int main() {

#if 1
// sample(how-printf-usage)
type_safe_printf(BOOST_HANA_STRING("%i, %f, %s"), 2, 3.4, "abcd");
// end-sample
#elif 0
// sample(how-printf-wrong_type)
type_safe_printf(BOOST_HANA_STRING("%i, %f, %s"), 2, 2, "abcd");
// end-sample
#elif 0
// sample(how-printf-wrong_narg)
type_safe_printf(BOOST_HANA_STRING("%i, %f, %s"), 2, 3.4);
// end-sample
#endif

}
