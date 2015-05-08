// Copyright Louis Dionne 2015
// Distributed under the Boost Software License, Version 1.0.

#include <boost/hana/assert.hpp>
#include <boost/hana/functional/fix.hpp>
#include <boost/hana/functional/partial.hpp>
#include <boost/hana/integral_constant.hpp>
#include <boost/hana/lazy.hpp>
#include <boost/hana/tuple.hpp>
using namespace boost::hana;


namespace disambiguate {
// sample(sorting-now)
template <typename Xs, typename Pred>
auto sort(Xs xs, Pred pred) {
  return eval_if(length(xs) < size_t<2>,
    lazy(xs),
    lazy([=](auto xs) {
      auto pivot = head(xs);
      auto parts = partition(tail(xs), partial(pred, pivot));
      return concat(
        append(sort(second(parts), pred), pivot),
        sort(first(parts), pivot)
      );
    })(xs)
  );
}
// end-sample
}

int main() {
  using disambiguate::sort;
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
