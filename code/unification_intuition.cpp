// Copyright Louis Dionne 2015
// Distributed under the Boost Software License, Version 1.0.

// sample(unification_intuition1)
template <typename T>
struct Type {
    // nothing
};

template <typename T>
Type<T*> add_pointer(Type<T>) {
    return {};
}
// end-sample

// sample(unification_intuition2)
template <typename T>
constexpr bool is_pointer(Type<T>) {
    return false;
}

template <typename T>
constexpr bool is_pointer(Type<T*>) {
    return true;
}
// end-sample


// sample(unification_intuition3)
Type<int> t{};
auto t_ptr = add_pointer(t);
static_assert(is_pointer(t_ptr), "");
// end-sample

int main() { }
