// Copyright Louis Dionne 2015
// Distributed under the Boost Software License, Version 1.0.

// sample(how-unify-Type)
template <typename T>
class Type { /* nothing */ };
// end-sample

// sample(how-unify-add_pointer)
template <typename T>
Type<T*> add_pointer(Type<T>) { return {}; }

template <typename T>
constexpr bool is_pointer(Type<T>) { return false; }

template <typename T>
constexpr bool is_pointer(Type<T*>) { return true; }
// end-sample

// sample(how-unify-usage)
Type<int> i{};
auto i_ptr = add_pointer(i);
static_assert(is_pointer(i_ptr), "");
// end-sample

namespace sugar {
// sample(how-unify-type)
template <typename T>
constexpr Type<T> type{};

auto i = type<int>;
auto i_ptr = add_pointer(i);
static_assert(is_pointer(i_ptr), "");
// end-sample
}

int main() { }
