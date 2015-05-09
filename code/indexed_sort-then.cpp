// Copyright Louis Dionne 2015
// Distributed under the Boost Software License, Version 1.0.

#include <boost/fusion/adapted/mpl.hpp>
#include <boost/fusion/include/as_vector.hpp>
#include <boost/fusion/include/at.hpp>

#include <boost/mpl/apply.hpp>
#include <boost/mpl/at.hpp>
#include <boost/mpl/back.hpp>
#include <boost/mpl/copy.hpp>
#include <boost/mpl/equal.hpp>
#include <boost/mpl/front.hpp>
#include <boost/mpl/lambda.hpp>
#include <boost/mpl/quote.hpp>
#include <boost/mpl/range_c.hpp>
#include <boost/mpl/sizeof.hpp>
#include <boost/mpl/sort.hpp>
#include <boost/mpl/transform.hpp>
#include <boost/mpl/vector.hpp>
#include <boost/mpl/vector_c.hpp>
#include <boost/mpl/zip_view.hpp>
namespace fusion = boost::fusion;
using namespace boost::mpl;


// sample(indexed_sort-then-impl)
template <typename Types, typename Predicate = quote2<less>>
struct indexed_sort {
  using indexed_types = typename copy<
    zip_view<vector<Types, range_c<long, 0l, size<Types>::value>>>,
    back_inserter<vector<>>
  >::type;

  using sorted = typename sort<
    indexed_types,
    apply2<typename lambda<Predicate>::type, front<_1>, front<_2>>
  >::type;

  using type = pair<
    typename transform<sorted, front<_1>>::type,
    typename transform<sorted, back<_1>>::type
  >;
};
// end-sample

int main() {
// sample(indexed_sort-then-usage)
using Types   = vector<int[3], int[2], int[1]>;
using Indexed = indexed_sort<Types, less<sizeof_<_1>,
                                         sizeof_<_2>>>::type;

using Sorted  = Indexed::first;
using Indices = Indexed::second;

static_assert(equal<Sorted, vector<int[1], int[2], int[3]>>::value, "");

static_assert(equal<Indices, vector_c<int, 2, 1, 0>,
  // mpl::equal does a shallow comparison without this
  quote2<equal_to>
>::value, "");
// end-sample

// sample(indexed_sort-then-fusion)
using Sequence = fusion::result_of::as_vector<Sorted>::type;
using IndexMap = indexed_sort<Indices>::type::second;

Sequence s;
int (&a)[3] = fusion::at_c<at_c<IndexMap, 0>::type::value>(s);
int (&b)[2] = fusion::at_c<at_c<IndexMap, 1>::type::value>(s);
int (&c)[1] = fusion::at_c<at_c<IndexMap, 2>::type::value>(s);
// end-sample

(void)a;
(void)b;
(void)c;
}
