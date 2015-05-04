// Copyright Louis Dionne 2015
// Distributed under the Boost Software License, Version 1.0.

#include <boost/fusion/include/at_key.hpp>
#include <boost/fusion/include/define_assoc_struct.hpp>

#include <string>
using namespace boost::fusion;


// sample(why-introspection-then)
namespace keys {
    struct name;
    struct age;
}

namespace demo { }
BOOST_FUSION_DEFINE_ASSOC_STRUCT( // must appear at global scope
    (demo), Person,
    (std::string, name, keys::name)
    (int, age, keys::age)
)

int main() {
    demo::Person john{"John", 30};
    std::string name = at_key<keys::name>(john);
    int age = at_key<keys::age>(john);
}
// end-sample
