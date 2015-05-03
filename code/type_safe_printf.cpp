// Copyright Louis Dionne 2015
// Distributed under the Boost Software License, Version 1.0.

#include <boost/hana.hpp>
#include <boost/hana/ext/std/type_traits.hpp>

#include <type_traits>
using namespace boost::hana;
using namespace boost::hana::literals;


namespace then {
// sample(type_safe_printf-then)

// end-sample
}

namespace now {
// sample(type_safe_printf-now2)
auto formats = make_map(
  make_pair(char_<'i'>, type<int>),
  make_pair(char_<'f'>, type<double>),
  make_pair(char_<'s'>, type<char*>)

  // ...
);
// end-sample

// sample(type_safe_printf-now3)
template <typename Fmt, typename ...Args>
int type_safe_printf(Fmt fmt, Args const& ...args) {
  static_assert(is_a<String>(fmt),
  "the format string must be a compile-time hana::String");

  // type-checking is done here, and then we just forward to printf.
  auto state = until(
    compose(less.than(size_t<2>), length, first),
    make_pair(fmt, tuple_t<std::decay_t<Args>...>),
    fuse([](auto fmt, auto args) {
      return eval_if(fmt[0_c] != char_<'%'>,
        [=] { return make_pair(tail(fmt), args); },
        lazy([](auto fmt, auto args) {
          static_assert(decltype(!is_empty(args)){},
          "too few arguments for the format string");

          static_assert(decltype(fmt[1_c] ^in^ formats){},
          "unsupported format character");

          static_assert(decltype(formats[fmt[1_c]] == just(args[0_c])){},
          "the type of the argument does not match the format character");

          return make_pair(drop(2_c, fmt), tail(args));
        })(fmt, args)
      );
    }));

  auto remaining_args = second(state);
  static_assert(is_empty(remaining_args),
  "too many arguments for the format string");

  return std::printf(to<char const*>(fmt),
                     static_cast<decltype(args)&&>(args)...);
};
// end-sample

int main() {

#if 1
// sample(type_safe_printf-now1)
type_safe_printf(BOOST_HANA_STRING("%i, %f, %s"), 2, 3.4, "abcd");
// end-sample
#elif 0
// sample(type_safe_printf-now-wrong_type)
type_safe_printf(BOOST_HANA_STRING("%i, %f, %s"), 2, 2, "abcd");
// end-sample
#elif 0
// sample(type_safe_printf-now-too_few_args)
type_safe_printf(BOOST_HANA_STRING("%i, %f, %s"), 2, 3.4);
// end-sample
#elif 0
// sample(type_safe_printf-now-too_many_args)
type_safe_printf(BOOST_HANA_STRING("%i, %f, %s"), 2, 3.4, "abcd", 44);
// end-sample
#endif

}

} // end namespace now

int main() { }
