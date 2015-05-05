// Copyright Louis Dionne 2015
// Distributed under the Boost Software License, Version 1.0.

#include <boost/hana.hpp>
#include <boost/hana/ext/std/type_traits.hpp>

#include <boost/mpl/copy_if.hpp>
#include <boost/mpl/equal.hpp>
#include <boost/mpl/or.hpp>
#include <boost/mpl/placeholders.hpp>
#include <boost/mpl/vector.hpp>

#include <boost/fusion/include/equal_to.hpp>
#include <boost/fusion/include/filter_if.hpp>
#include <boost/fusion/include/make_vector.hpp>

#include <type_traits>
namespace fusion = boost::fusion;
namespace mpl = boost::mpl;


int main() {

{

using mpl::_1;

// sample(how-onelib-mpl)
// types (MPL)
using ts = mpl::vector<int, char&, void*>;
using us = mpl::copy_if<ts, mpl::or_<std::is_pointer<_1>,
                                     std::is_reference<_1>>>::type;

// values (Fusion)
auto vs = fusion::make_vector(1, 'c', nullptr, 3.5);
auto ws = fusion::filter_if<std::is_integral<mpl::_1>>(vs);
// end-sample

static_assert(mpl::equal<us, mpl::vector<char&, void*>>::value, "");
BOOST_HANA_RUNTIME_CHECK(ws == fusion::make_vector(1, 'c'));

}{

using namespace boost::hana;
using namespace boost::hana::traits;

// sample(how-onelib-hana)
// types
auto ts = make_tuple(type<int>, type<char&>, type<void*>);
auto us = filter(ts, [](auto t) {
  return or_(is_pointer(t), is_reference(t));
});

// values
auto vs = make_tuple(1, 'c', nullptr, 3.5);
auto ws = filter(vs, [](auto t) {
  return is_integral(type<decltype(t)>);
});
// end-sample

BOOST_HANA_CONSTANT_CHECK(us == make_tuple(type<char&>, type<void*>));
BOOST_HANA_RUNTIME_CHECK(ws == make_tuple(1, 'c'));

}{

using namespace boost::hana;
using namespace boost::hana::traits;

// sample(how-onelib-hana-sugar)
// types
auto ts = tuple_t<int, char&, void*>;
auto us = filter(ts, [](auto t) {
  return or_(is_pointer(t), is_reference(t));
});

// values
auto vs = make_tuple(1, 'c', nullptr, 3.5);
auto ws = filter(vs, [](auto t) {
  return is_integral(t);
});
// end-sample

BOOST_HANA_CONSTANT_CHECK(us == make_tuple(type<char&>, type<void*>));
BOOST_HANA_RUNTIME_CHECK(ws == make_tuple(1, 'c'));

}

}
