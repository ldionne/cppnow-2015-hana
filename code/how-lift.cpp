// Copyright Louis Dionne 2015
// Distributed under the Boost Software License, Version 1.0.

#include <type_traits>


template <typename T>
class Type { };

template <typename T>
constexpr Type<T> type{};

template <typename T>
constexpr bool is_pointer(Type<T>) { return false; }

template <typename T>
constexpr bool is_pointer(Type<T*>) { return true; }

// sample(how-lift)
template <template <typename ...> class F>
struct Metafunction {
    template <typename ...T>
    auto operator()(Type<T> ...) const {
        return type<
            typename F<T...>::type
        >;
    }
};

Metafunction<std::add_pointer> add_pointer{};

auto i = type<int>;
auto i_ptr = add_pointer(i);
static_assert(is_pointer(i_ptr), "");
// end-sample

namespace sugar {
// sample(how-lift-sugar)
template <template <typename ...> class F>
constexpr Metafunction<F> metafunction{};

auto add_pointer = metafunction<std::add_pointer>;
auto remove_reference = metafunction<std::remove_reference>;
auto add_const = metafunction<std::add_const>;
// ...
// end-sample
}

int main() { }
