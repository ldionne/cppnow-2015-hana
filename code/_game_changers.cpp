// Copyright Louis Dionne 2015
// Distributed under the Boost Software License, Version 1.0.

#include <sstream>
#include <string>


namespace generalized_constexpr {
namespace then {
// sample(game_changers-generalized_constexpr-then)
constexpr unsigned long long factorial(unsigned long long n) {
    return n == 0 ? 1 : n * factorial(n-1);
}
// end-sample
}

namespace now {
// sample(game_changers-generalized_constexpr-now)
constexpr unsigned long long factorial(unsigned long long n) {
    unsigned long long result = 1;
    while (n != 0)
        result *= n--;
    return result;
}
// end-sample
}
}

namespace generic_lambdas {
namespace then {
// sample(game_changers-generic_lambdas-then)
struct to_string {
    template <typename X>
    std::string operator()(X x) const {
        std::ostringstream s;
        s << x;
        return s.str();
    }
};

std::string x = to_string{}(1.3);
// end-sample
}

namespace now {
// sample(game_changers-generic_lambdas-now)
auto to_string = [](auto x) -> std::string {
    std::ostringstream s;
    s << x;
    return s.str();
};

std::string x = to_string(1.3);
// end-sample
}
}

namespace auto_return_type {
namespace then {
// sample(game_changers-auto_return_type-then)
template <typename F, typename ...X>
auto apply(F f, X ...x)
    -> decltype(f(x...)) // must be a single expression
{
    return f(x...);
}
// end-sample
}

namespace now {
// sample(game_changers-auto_return_type-now)
template <typename F, typename ...X>
auto apply(F f, X ...x)
    // just leave it out
{
    return f(x...);
}
// end-sample
}
}


int main() { }
