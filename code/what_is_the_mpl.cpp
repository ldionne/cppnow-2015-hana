// Copyright Louis Dionne 2015
// Distributed under the Boost Software License, Version 1.0.

// sample(what_is_the_mpl-metafunction)
template <typename T>
struct add_pointer {
    using type = T*;
};

using result = add_pointer<int>::type;
// end-sample

// sample(what_is_the_mpl-sequence)
template <typename ...T>
struct vector;

using types = vector<int, char, float, void>;
// end-sample


namespace ns1 {
// sample(what_is_the_mpl-algorithm-front)
template <typename Vector>
struct front;

template <typename T, typename ...Ts>
struct front<vector<T, Ts...>> {
    using type = T;
};

using types = vector<int, char, float, void>;
using first = front<types>::type; // int
// end-sample
}

namespace ns2 {
// sample(what_is_the_mpl-algorithm-transform)
template <typename Vector, template <typename ...> class Metafunction>
struct transform;

template <typename ...T, template <typename ...> class Metafunction>
struct transform<vector<T...>, Metafunction> {
    using type = vector<typename Metafunction<T>::type...>;
};

using types = vector<int, char, float, void>;
using pointers = transform<types, add_pointer>::type;
// vector<int*, char*, float*, void*>
// end-sample
}


int main() { }
