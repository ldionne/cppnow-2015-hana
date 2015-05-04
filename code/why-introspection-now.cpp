// Copyright Louis Dionne 2015
// Distributed under the Boost Software License, Version 1.0.

#include <boost/hana/string.hpp>
#include <boost/hana/struct.hpp>
#include <boost/hana/comparable.hpp>
#include <boost/hana/struct_macros.hpp>

#include <cassert>
#include <string>
using namespace boost::hana;


// sample(why-introspection-now)
namespace demo {
    struct Person {
        BOOST_HANA_DEFINE_STRUCT(Person,
            (std::string, name),
            (int, age)
        );
    };
}

int main() {
    demo::Person john{"John", 30};
    std::string name = at_key(john, BOOST_HANA_STRING("name"));
    int age = at_key(john, BOOST_HANA_STRING("age"));
}
// end-sample
