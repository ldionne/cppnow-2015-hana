// Copyright Louis Dionne 2015
// Distributed under the Boost Software License, Version 1.0.

#include <boost/hana.hpp>
#include <boost/hana/ext/std/type_traits.hpp>

#include <boost/mpl/copy_if.hpp>
#include <boost/mpl/equal.hpp>
#include <boost/mpl/or.hpp>
#include <boost/mpl/placeholders.hpp>
#include <boost/mpl/vector.hpp>

#include <type_traits>


int main() {

{

using namespace boost::mpl;

// sample(how-full_language-mpl)
using ts = vector<int, char&, void*>;
using us = copy_if<ts, or_<std::is_pointer<_1>,
                           std::is_reference<_1>>>::type;
// end-sample

static_assert(equal<us, vector<char&, void*>>::value, "");

}{

using namespace boost::hana;
using namespace boost::hana::traits;

// sample(how-full_language-hana)
auto ts = make_tuple(type<int>, type<char&>, type<void*>);
auto us = filter(ts, [](auto t) {
  return or_(is_pointer(t), is_reference(t));
});
// end-sample

BOOST_HANA_CONSTANT_CHECK(us == make_tuple(type<char&>, type<void*>));

}

}
