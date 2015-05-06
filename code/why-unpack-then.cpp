// Copyright Louis Dionne 2015
// Distributed under the Boost Software License, Version 1.0.

#include <cstddef>
#include <iostream>
#include <tuple>
#include <type_traits>
#include <utility>


// sample(why-unpack-then)
template <typename Tuple, typename F, std::size_t ...i>
constexpr decltype(auto)
unpack_impl(Tuple&& tuple, F&& f, std::index_sequence<i...>) {
  return std::forward<F>(f)(std::get<i>(std::forward<Tuple>(tuple))...);
}

template <typename Tuple, typename F>
constexpr decltype(auto) unpack(Tuple&& tuple, F&& f) {
    constexpr std::size_t n = std::tuple_size<
        std::remove_reference_t<Tuple>
    >::value;
    return unpack_impl(std::forward<Tuple>(tuple), std::forward<F>(f),
                       std::make_index_sequence<n>{});
}

void f(int a, double b, std::string c) {
  std::cout << a << ":" << b << ":" << c << std::endl;
}

int main() {
  unpack(std::make_tuple(1, 3.4, "abcdef"), f);
}
// end-sample
