// Copyright Louis Dionne 2015
// Distributed under the Boost Software License, Version 1.0.

#include <boost/hana.hpp>
#include <array>
using namespace boost::hana;


template <typename T>
void visit(T t) { }

template <typename ...T>
std::array<void(*)(void*), sizeof...(T)> function_table = {{
  [](void* v) -> decltype(auto) {
    return visit<T>(*static_cast<T*>(v));
  }...
}};

int main() {
  auto table = function_table<int, float, char>;
  auto visit_as_int = table[0];
  auto visit_as_float = table[1];
  auto visit_as_char = table[2];

#if 0 // just ignore this
  auto function_map = [](auto ...types) {
    return to<Map>(transform(make_tuple(types...), [](auto t) {
      using T = typename decltype(t)::type;
      return make_pair(type<T>, [](void* v) -> decltype(auto) {
        return visit<T>(*static_cast<T*>(v));
      });
    }));
  };

  auto functions = function_map(type<int>, type<float>, type<char>);
  void* v = nullptr;
  functions[type<int>](v);
#endif
}
