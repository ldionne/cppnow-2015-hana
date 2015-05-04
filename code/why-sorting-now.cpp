// Copyright Louis Dionne 2015
// Distributed under the Boost Software License, Version 1.0.

#include <boost/hana.hpp>
using namespace boost::hana;


// sample(why-sorting-now)
auto sort = fix([](auto sort, auto xs, auto pred) {
  return eval_if(length(xs) < size_t<2>,
    lazy(xs),
    lazy([=](auto xs) {
      auto pivot = head(xs);
      auto rest = tail(xs);
      auto parts = partition(rest, partial(pred, pivot));
      return concat(
        sort(second(parts), pred),
        prepend(pivot, sort(first(parts), pivot))
      );
    })(xs)
  );
});
// end-sample

int main() {
  using ::sort;
  BOOST_HANA_CONSTANT_CHECK(
    sort(make_tuple(), less) == make_tuple()
  );

  BOOST_HANA_CONSTANT_CHECK(
    sort(make_tuple(int_<1>), less) == make_tuple(int_<1>)
  );

  BOOST_HANA_CONSTANT_CHECK(
    sort(make_tuple(int_<2>, int_<1>), less) == make_tuple(int_<1>, int_<2>)
  );

  BOOST_HANA_CONSTANT_CHECK(
    sort(make_tuple(int_<3>, int_<2>, int_<1>), less) ==
      make_tuple(int_<1>, int_<2>, int_<3>)
  );

  BOOST_HANA_CONSTANT_CHECK(
    sort(make_tuple(int_<4>, int_<3>, int_<2>, int_<1>), less) ==
      make_tuple(int_<1>, int_<2>, int_<3>, int_<4>)
  );
}
