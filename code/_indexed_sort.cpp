// Copyright Louis Dionne 2015
// Distributed under the Boost Software License, Version 1.0.

#include <boost/fusion/adapted/mpl.hpp>
#include <boost/fusion/include/as_vector.hpp>
#include <boost/fusion/include/at.hpp>
#include <boost/mpl/apply.hpp>
#include <boost/mpl/at.hpp>
#include <boost/mpl/back.hpp>
#include <boost/mpl/copy.hpp>
#include <boost/mpl/front.hpp>
#include <boost/mpl/lambda.hpp>
#include <boost/mpl/quote.hpp>
#include <boost/mpl/range_c.hpp>
#include <boost/mpl/sizeof.hpp>
#include <boost/mpl/sort.hpp>
#include <boost/mpl/transform.hpp>
#include <boost/mpl/vector.hpp>
#include <boost/mpl/zip_view.hpp>

#include <boost/hana.hpp>
namespace fusion = boost::fusion;
namespace hana = boost::hana;
namespace mpl = boost::mpl;


namespace then {
// sample(indexed_sort-then)
template <typename Types, typename Predicate = mpl::quote2<mpl::less>>
struct indexed_sort {
    using indexed_types = typename mpl::copy<
        mpl::zip_view<mpl::vector<
            Types,
            mpl::range_c<long, 0l, mpl::size<Types>::value>
        >>,
        mpl::back_inserter<mpl::vector<>>
    >::type;

    using sorted_indices = typename mpl::sort<
        indexed_types,
        mpl::apply2<typename mpl::lambda<Predicate>::type,
                    mpl::front<mpl::_1>, mpl::front<mpl::_2>>
    >::type;

    using type = mpl::pair<
        typename mpl::transform<sorted_indices, mpl::front<mpl::_1>>::type,
        typename mpl::transform<sorted_indices, mpl::back<mpl::_1>>::type
    >;
};

struct A { long s[1]; };
struct B { long s[2]; };
struct C { long s[3]; };
struct D { long s[4]; };

using sorted = indexed_sort<mpl::vector<D, C, B, A>,
    mpl::less<mpl::sizeof_<mpl::_1>, mpl::sizeof_<mpl::_2>>
>::type;

using Sequence = fusion::result_of::as_vector<sorted::first>::type;
using indices = indexed_sort<sorted::second>::type::second;

void test() {
    Sequence s;
    D d = fusion::at_c<mpl::at_c<indices, 0>::type::value>(s);
    C c = fusion::at_c<mpl::at_c<indices, 1>::type::value>(s);
    B b = fusion::at_c<mpl::at_c<indices, 2>::type::value>(s);
    A a = fusion::at_c<mpl::at_c<indices, 3>::type::value>(s);
}
// end-sample
}

namespace now {
// sample(indexed_sort-now)
using namespace boost::hana::literals;
auto indexed_sort = [](auto list, auto predicate) {
    auto indices = hana::to<hana::Tuple>(hana::range(0_c, hana::size(list)));
    auto indexed_list = hana::zip(list, indices);
    auto sorted = hana::sort.by(predicate ^hana::on^ hana::head, indexed_list);
    return hana::make_pair(
        hana::transform(sorted, hana::head),
        hana::transform(sorted, hana::last)
    );
};

struct A { long s[1]; };
struct B { long s[2]; };
struct C { long s[3]; };
struct D { long s[4]; };

auto sorted = indexed_sort(hana::tuple_t<D, C, B, A>, [](auto t, auto u) {
    return hana::sizeof_(t) < hana::sizeof_(u);
});

using Sequence = decltype(
    hana::unpack(hana::first(sorted), hana::template_<hana::_tuple>)
)::type;
auto indices = hana::second(indexed_sort(hana::second(sorted), hana::less));

void test() {
    Sequence s;
    D d = s[indices[0_c]];
    C c = s[indices[1_c]];
    B b = s[indices[2_c]];
    A a = s[indices[3_c]];
}
// end-sample
}

int main() { }
